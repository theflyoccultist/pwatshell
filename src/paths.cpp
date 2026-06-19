#include "paths.hpp"
#include "str.hpp"
#include <array>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

constexpr char PATH_LIST_SEPARATOR = ':';
constexpr size_t PATH_MAX = 1024;

bool Paths::isExecutable(const fs::path &candidate) const {
    if (fs::exists(candidate)) {
        auto perms = fs::status(candidate).permissions();

        return (perms & fs::perms::owner_exec) != fs::perms::none ||
               (perms & fs::perms::group_exec) != fs::perms::none ||
               (perms & fs::perms::others_exec) != fs::perms::none;
    }

    return false;
}

fs::path Paths::getExecutablePath(const std::string &cmd) const {
    for (const auto &dir : pathList) {
        fs::path candidate = fs::path(dir) / cmd;

        if (this->isExecutable(candidate)) {
            return candidate;
        }
    }

    return "";
}

std::vector<std::string> Paths::getExecutablesInPathEnv() const {
    std::vector<std::string> executableList;

    for (auto &pathEnv : pathList) {
        for (auto const &dir_entry : fs::directory_iterator{pathEnv}) {
            const fs::path &candidate = dir_entry.path();
            if (this->isExecutable(candidate)) {
                executableList.emplace_back(candidate.filename().string());
            }
        }
    }

    return executableList;
}

std::vector<std::string> Paths::getFilesInCurrPath() const {
    std::vector<std::string> fileList;
    const std::string &pwd = this->pwd();

    for (auto const &dir_entry : fs::recursive_directory_iterator{pwd}) {
        const fs::path &candidate = dir_entry.path();
        if (!this->isExecutable(candidate) && fs::is_regular_file(candidate)) {
            std::string file = candidate.string();
            str::eraseCommonSubString(file, pwd + '/');
            fileList.emplace_back(file);
        }
    }

    return fileList;
}

std::string Paths::pwd() const {
    std::array<char, PATH_MAX> buffer{};
    if (::getcwd(buffer.data(), buffer.size()) == nullptr) {
        throw std::runtime_error("getcwd failed");
    }
    return buffer.data();
}

void Paths::changeDirectory(std::string path) {
    if (!path.empty() && path[0] == '~') {
        std::string homeEnv = this->getPathHome();

        if (path.length() == 1) {
            path = homeEnv;
        } else if (path[1] == '/') {
            path = homeEnv + path.substr(1);
        }
    }

    if (::chdir(path.c_str()) != 0) {
        std::cerr << "cd: " << path << ": No such file or directory\n";
        return;
    }
}

std::string Paths::getPathEnv() {
    const char *path = std::getenv("PATH");
    std::string path_str;

    if (path != nullptr) {
        path_str = path;
    } else {
        path_str = "";
    }

    return path_str;
}

std::string Paths::getPathHome() {
    const char *path = std::getenv("HOME");
    std::string path_str;

    if (path != nullptr) {
        path_str = path;
    } else {
        path_str = "";
    }

    return path_str;
}

std::vector<std::string> Paths::generatePathList() {
    std::string pathEnv = getPathEnv();
    std::vector<std::string> pathList = str::splitString(pathEnv, PATH_LIST_SEPARATOR);
    return pathList;
}
