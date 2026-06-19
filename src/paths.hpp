#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class Paths {
  public:
    Paths() : currentPath(fs::current_path()), pathList(generatePathList()) {}

    [[nodiscard]] fs::path getCurrentPath() const;
    [[nodiscard]] fs::path getExecutablePath(const std::string &cmd) const;
    [[nodiscard]] std::vector<std::string> getExecutablesInPathEnv() const;
    [[nodiscard]] std::vector<std::string> getFilesInCurrPath() const;

    std::string pwd();
    void changeDirectory(std::string path);

  private:
    static std::string getPathEnv();
    static std::string getPathHome();
    static std::vector<std::string> generatePathList();

    fs::path currentPath;
    std::vector<std::string> pathList;

    [[nodiscard]] bool isExecutable(const fs::path &candidate) const;
};
