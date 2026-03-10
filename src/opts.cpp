#include "opts.hpp"
#include <unordered_map>

namespace opts {
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
} // namespace opts
