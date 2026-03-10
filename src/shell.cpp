#include "opts.hpp"
#include "str.hpp"

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace shell {
void invalid(std::string &command) {
  std::cerr << command << ": command not found\n";
}
void echo(std::string &command) { std::cout << str::rtrim(command) << "\n"; }

std::vector<std::string> generatePathList() {
  std::string pathEnv = str::getPathEnv();
  std::vector<std::string> pathList = str::splitPath(pathEnv);
  return pathList;
}

static const std::vector<std::string> pathList = generatePathList();

std::string getExecutablePath(const std::string &cmd) {
  for (const auto &dir : pathList) {
    fs::path candidate = fs::path(dir) / cmd;

    if (fs::exists(candidate)) {
      auto perms = fs::status(candidate).permissions();

      bool isExecutable = (perms & fs::perms::owner_exec) != fs::perms::none ||
                          (perms & fs::perms::group_exec) != fs::perms::none ||
                          (perms & fs::perms::others_exec) != fs::perms::none;

      if (isExecutable) {
        return candidate.string();
      }
    }
  }

  return "";
}

void type(std::string &command) {
  std::string userInput = str::rtrim(command);

  if (opts::resolveOption(userInput) != Options::Invalid) {
    std::cout << userInput << " is a shell builtin\n";
  } else if (getExecutablePath(userInput) != "") {
    std::cout << userInput << " is " << getExecutablePath(userInput) << "\n";
  } else {
    std::cout << userInput << ": not found\n";
  }
}
} // namespace shell
