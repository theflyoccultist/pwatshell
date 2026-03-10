#pragma once

#include <string>

namespace str {
static inline std::string ltrim(std::string &s) {
  int space = s.find(' ');
  return s.substr(0, space);
}

static inline std::string rtrim(std::string &s) {
  int space = s.find(' ');
  return s.substr(space + 1, s.length());
}
} // namespace str
