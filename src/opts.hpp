#pragma once

#include "shell.hpp"
#include <cstdint>
#include <string>
#include <unordered_map>

enum class Options : uint8_t {
  Echo,
  Type,
  Executable,
  Exit,
  Invalid,
};

namespace opts {
static Options resolveOption(const std::string &input) {
  static std::unordered_map<std::string, Options> optionsMap = {
      {"echo", Options::Echo},
      {"type", Options::Type},
      {Shell::executableCmds(input)[0], Options::Executable},
      {"exit", Options::Exit},
  };

  auto it = optionsMap.find(input);
  if (it != optionsMap.end()) {
    return it->second;
  }
  return Options::Invalid;
}
} // namespace opts
