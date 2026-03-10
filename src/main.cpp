#include <algorithm>
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

static inline std::string &ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                  [](int c) { return !std::isspace(c); }));
  return s;
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

    switch (resolveOption(command)) {
    case Options::Echo:
      std::cout << ltrim(command) << "\n";
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
