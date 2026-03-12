#include "paths.hpp"
#include "str.hpp"
#include <array>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unistd.h>

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

std::string Paths::pwd() {
  std::array<char, PATH_MAX> buffer{};
  if (::getcwd(buffer.data(), buffer.size()) == nullptr) {
    throw std::runtime_error("getcwd failed");
  }
  return std::string(buffer.data());
}

void Paths::changeDirectory(const std::string &path) {
  if (chdir(path.c_str()) != 0) {
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

std::vector<std::string> Paths::generatePathList() {
  std::string pathEnv = getPathEnv();
  std::vector<std::string> pathList =
      str::splitString(pathEnv, PATH_LIST_SEPARATOR);
  return pathList;
}
