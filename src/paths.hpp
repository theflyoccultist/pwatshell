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
  fs::path getExecutablePath(const std::string &cmd);

private:
  static std::string getPathEnv();
  static std::vector<std::string> splitPath(const std::string &path);
  static std::vector<std::string> generatePathList();
  static const std::vector<std::string> pathList;
};
