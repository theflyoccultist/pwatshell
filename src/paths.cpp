#include "paths.hpp"
#include "str.hpp"
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

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

void Paths::changeDirectory(const std::string &path) {
  try {
    fs::path newPath = fs::canonical(path);
    currentPath = newPath;
  } catch (const fs::filesystem_error &e) {
    std::cerr << "cd: " << e.what() << "\n";
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
