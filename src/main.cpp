#include <iostream>
#include <string>

int main() {
  // Flush after every std::cout / std:cerr
  while (true) {
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    std::cout << "$ ";
    std::string command;
    std::getline(std::cin, command);
    std::cerr << command << ": command not found";
  }
}
