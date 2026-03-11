#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

#ifdef _WIN32
constexpr char PATH_LIST_SEPARATOR = ';';
#else
constexpr char PATH_LIST_SEPARATOR = ':';
#endif

class Paths {
public:
  static fs::path getExecutablePath(const std::string &cmd);
  static const std::vector<std::string> pathList;
  static fs::path getCurrentPath();
  static void changeDirectory(const std::string &path);

private:
  static std::string getPathEnv();
  static std::vector<std::string> generatePathList();
  static fs::path currentPath;
};
