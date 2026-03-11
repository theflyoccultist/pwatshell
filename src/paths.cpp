#include "paths.hpp"
#include "str.hpp"
#include <cstdlib>
#include <filesystem>
#include <string>

Paths::Paths() { currentPath = fs::current_path(); }

fs::path Paths::getExecutablePath(const std::string &cmd) {
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

const std::vector<std::string> Paths::pathList = Paths::generatePathList();

fs::path Paths::getCurrentPath() { return currentPath; }

int Paths::changeDirectory(const std::string &path) {
  fs::path newPath = fs::path(path);
  if (fs::is_directory(newPath)) {
    currentPath = newPath;
    return EXIT_SUCCESS;
  }
  return EXIT_FAILURE;
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
