#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class Paths {
  public:
    Paths() : pathList(generatePathList()) {}

    [[nodiscard]] fs::path getExecutablePath(const std::string &cmd) const;
    [[nodiscard]] std::vector<std::string> getExecutablesInPathEnv() const;

    [[nodiscard]] std::string pwd() const;
    void changeDirectory(std::string path) const;

    static std::string getPathEnv();
    static std::string getPathHome();
    static const char *getPathHist();

  private:
    static std::vector<std::string> generatePathList();

    std::vector<std::string> pathList;

    [[nodiscard]] bool isExecutable(const fs::path &candidate) const;
};
