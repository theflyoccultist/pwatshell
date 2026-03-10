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

    if (command == "exit") {
      running = false;
    } else {
      std::cerr << command << ": command not found\n";
    }
  }
}
