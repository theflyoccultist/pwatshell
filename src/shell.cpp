#include "shell.hpp"
#include "opts.hpp"
#include "paths.hpp"
#include "str.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

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

std::vector<char *> Shell::executableCmds(const std::string &command) {
  std::vector<std::string> args = str::splitString(command, ' ');
  std::vector<char *> argv;

  argv.reserve(args.size() + 1);

  for (auto &arg : args) {
    argv.push_back(arg.data());
  }

  argv.push_back(nullptr);

  return argv;
}

int Shell::executable(std::string &command) {
  std::vector<char *> argv = executableCmds(command);

  pid_t pid = fork();

  if (pid < 0) {
    perror("fork failed");
    return EXIT_FAILURE;
  }

  if (pid == 0) {
    execvp(argv[0], argv.data());
    perror("exec failed");
    exit(EXIT_FAILURE);
  } else {
    waitpid(pid, nullptr, 0);
    return EXIT_SUCCESS;
  }
}
