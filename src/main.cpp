#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>

enum class Options : uint8_t {
  Echo,
  Exit,
  Invalid,
};

Options resolveOption(std::string input) {
  static const std::unordered_map<std::string, Options> optionsMap = {
      {"echo", Options::Echo},
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
  return s.substr(space, s.length());
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
    case Options::Exit:
      running = false;
      break;
    case Options::Invalid:
      std::cerr << command << ": command not found\n";
      break;
    }
  }
}
