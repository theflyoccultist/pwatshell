#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

constexpr char PATH_LIST_SEPARATOR = ':';
constexpr size_t PATH_MAX = 1024;

class Paths {
  public:
    Paths() : currentPath(fs::current_path()), pathList(generatePathList()) {}

    [[nodiscard]] fs::path getCurrentPath() const;
    [[nodiscard]] fs::path getExecutablePath(const std::string &cmd) const;

    std::string pwd();
    void changeDirectory(std::string path);

  private:
    static std::string getPathEnv();
    static std::string getPathHome();
    static std::vector<std::string> generatePathList();

    fs::path currentPath;
    std::vector<std::string> pathList;
};
