#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include "fileinfo.hpp"

namespace fs = std::filesystem;

class Paths {
  public:
    Paths() : pathList(generatePathList()) {}

    [[nodiscard]] fs::path getExecutablePath(const std::string &cmd) const;
    [[nodiscard]] std::vector<FileInfo> getExecutablesInPathEnv() const;
    [[nodiscard]] std::vector<FileInfo> getFilesInCurrPath() const;

    [[nodiscard]] std::string pwd() const;
    void changeDirectory(std::string path);

  private:
    static std::string getPathEnv();
    static std::string getPathHome();
    static std::vector<std::string> generatePathList();

    std::vector<std::string> pathList;

    [[nodiscard]] bool isExecutable(const fs::path &candidate) const;
};
