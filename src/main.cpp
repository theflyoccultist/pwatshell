#include "opts.hpp"
#include "str.hpp"
#include <iostream>
#include <string>

void typeOption(std::string command) {
  std::string userInput = str::rtrim(command);

  if (opts::resolveOption(userInput) != Options::Invalid) {
    std::cout << userInput << " is a shell builtin\n";
  } else {
    std::cout << userInput << ": not found\n";
  }
}

int main() {
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
      std::cout << str::rtrim(command) << "\n";
      break;
    case Options::Type:
      typeOption(command);
      break;
    case Options::Exit:
      running = false;
      break;
    case Options::Invalid:
      std::cerr << command << ": command not found\n";
      break;
    }
  }
}
