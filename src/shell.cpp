#include "shell.hpp"
#include "opts.hpp"
#include "str.hpp"

#include <iostream>
#include <string>

void Shell::invalid(std::string &command) {
  std::cerr << command << ": command not found\n";
}

void Shell::echo(std::string &command) {
  std::cout << str::rtrim(command) << "\n";
}

void Shell::type(std::string &command) {
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
