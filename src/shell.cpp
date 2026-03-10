#include "opts.hpp"
#include "str.hpp"

#include <algorithm>
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

std::vector<std::string> getExecutableList() {
  std::string pathEnv = str::getPath();
  std::vector<std::string> pathList = str::splitPath(pathEnv);
  std::vector<std::string> exeList;

  for (const auto &p : pathList) {
    auto dirPath = fs::path(p);

    for (const auto &entry : fs::directory_iterator(dirPath)) {
      fs::file_status fileStatus = fs::status(entry);
      bool isExecutable =
          (fileStatus.permissions() & fs::perms::owner_exec) != fs::perms::none;

      if (isExecutable) {
        exeList.push_back(entry.path().string());
      }
    }
  }

  return exeList;
}

std::string getExecutablePath(const std::string &userInput) {
  const std::vector<std::string> exeList = getExecutableList();
  const auto it = std::find_if(exeList.begin(), exeList.end(), userInput);
  if (it != exeList.end()) {
    return *it;
  }
  return "hello";
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
