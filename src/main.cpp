#include "opts.hpp"
#include "shell.hpp"
#include "str.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

int main() {
  const char *path = std::getenv("PATH");

  if (path) {
    std::string path_str = path;
    std::cout << "PATH = " << path_str << "\n";
  } else {
    std::cout << "PATH is not set\n";
  }

  bool running = true;

  while (running) {
    // Flush after every std::cout / std:cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    std::cout << "$ ";
    std::string command;
    std::getline(std::cin, command);

    switch (opts::resolveOption(str::ltrim(command))) {
    case Options::Echo:
      shell::echo(command);
      break;
    case Options::Type:
      shell::type(command);
      break;
    case Options::Exit:
      running = false;
      break;
    case Options::Invalid:
      shell::invalid(command);
      break;
    }
  }
}
