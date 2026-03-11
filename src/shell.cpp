#include "shell.hpp"
#include "opts.hpp"
#include "paths.hpp"
#include "str.hpp"

#include <cstring>
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

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
  } else if (Paths::getExecutablePath(userInput) != "") {
    std::cout << userInput << " is "
              << Paths::getExecutablePath(userInput).string() << "\n";
  } else {
    std::cout << userInput << ": not found\n";
  }
}

void Shell::executable(std::string &command) {
  std::string candidate = str::ltrim(command);
  const char *arg0 = Paths::getExecutablePath(candidate).c_str();

  std::vector<std::string> args = str::splitString(command, ' ');
  std::vector<char *> argv;

  argv.reserve(args.size());
  for (auto &arg : args) {
    argv.push_back(arg.data());
  }

  if (strcmp(arg0, "") != 0) {
    execvp(arg0, argv.data());
  }
}
