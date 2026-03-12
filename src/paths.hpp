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
  Paths() {
    currentPath = fs::current_path();
    pathList = generatePathList();
  }

  [[nodiscard]] fs::path getExecutablePath(const std::string &cmd) const;
  [[nodiscard]] fs::path getCurrentPath() const;
  void changeDirectory(const std::string &path);

private:
  static std::string getPathEnv();
  std::vector<std::string> generatePathList();

  fs::path currentPath;
  std::vector<std::string> pathList;
};
