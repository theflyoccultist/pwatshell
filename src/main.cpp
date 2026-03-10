#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>

enum class Options : uint8_t {
  Echo,
  Type,
  Exit,
  Invalid,
};

Options resolveOption(std::string input) {
  static const std::unordered_map<std::string, Options> optionsMap = {
      {"echo", Options::Echo},
      {"type", Options::Type},
      {"exit", Options::Exit},
  };

  auto it = optionsMap.find(input);
  if (it != optionsMap.end()) {
    return it->second;
  }
  return Options::Invalid;
}

std::string ltrim(std::string &s) {
  int space = s.find(' ');
  return s.substr(0, space);
}

std::string rtrim(std::string &s) {
  int space = s.find(' ');
  return s.substr(space + 1, s.length());
}

void typeOption(std::string command) {
  std::string userInput = rtrim(command);

  if (resolveOption(userInput) != Options::Invalid) {
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

    switch (resolveOption(ltrim(command))) {
    case Options::Echo:
      std::cout << rtrim(command) << "\n";
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
