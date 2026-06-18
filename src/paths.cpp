#include "paths.hpp"
#include "str.hpp"
#include <array>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <vector>

fs::path Paths::getCurrentPath() const { return currentPath; }

fs::path Paths::getExecutablePath(const std::string &cmd) const {
    for (const auto &dir : pathList) {
        fs::path candidate = fs::path(dir) / cmd;

        if (fs::exists(candidate)) {
            auto perms = fs::status(candidate).permissions();

            bool isExecutable = (perms & fs::perms::owner_exec) != fs::perms::none ||
                                (perms & fs::perms::group_exec) != fs::perms::none ||
                                (perms & fs::perms::others_exec) != fs::perms::none;

            if (isExecutable) {
                return candidate;
            }
        }
    }

    return "";
}

std::vector<std::string> Paths::getExecutablesInPathEnv() {
    std::vector<std::string> executableList;

    for (auto &pathEnv : pathList) {
        for (auto const &dir_entry : fs::directory_iterator{pathEnv}) {
            fs::file_status fileStatus = fs::status(dir_entry.path());

            if (fs::exists(fileStatus) &&
                (fileStatus.permissions() & fs::perms::owner_exec) != fs::perms::none)
                executableList.emplace_back(dir_entry.path().filename().string());
        }
    }

    return executableList;
}

std::string Paths::pwd() {
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
