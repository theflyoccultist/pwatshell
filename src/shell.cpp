#include "opts.hpp"
#include "paths.hpp"
#include "str.hpp"

#include <iostream>
#include <string>

namespace shell {
Paths paths;

void invalid(std::string &command) {
  std::cerr << command << ": command not found\n";
}

void echo(std::string &command) { std::cout << str::rtrim(command) << "\n"; }

void type(std::string &command) {
  std::string userInput = str::rtrim(command);

  if (opts::resolveOption(userInput) != Options::Invalid) {
    std::cout << userInput << " is a shell builtin\n";
  } else if (paths.getExecutablePath(userInput) != "") {
    std::cout << userInput << " is "
              << paths.getExecutablePath(userInput).string() << "\n";
  } else {
    std::cout << userInput << ": not found\n";
  }
}
} // namespace shell
