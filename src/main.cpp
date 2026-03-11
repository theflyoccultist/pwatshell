#include "opts.hpp"
#include "shell.hpp"
#include "str.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

int main() {

  bool running = true;

  while (running) {
    // Flush after every std::cout / std:cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    std::cout << "$ ";
    std::string command;
    std::getline(std::cin, command);

    if (Shell::executable(command) == EXIT_FAILURE) {

      switch (opts::resolveOption(str::ltrim(command))) {
      case Options::Echo:
        Shell::echo(command);
        break;
      case Options::Type:
        Shell::type(command);
        break;
      case Options::Exit:
        running = false;
        break;
      case Options::Invalid:
        Shell::invalid(command);
        break;
      }
    }
  }
}
