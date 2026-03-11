#include "str.hpp"

namespace str {
std::string ltrim(std::string &s) {
  unsigned long space = s.find(' ');
  return s.substr(0, space);
}

std::string rtrim(std::string &s) {
  unsigned long space = s.find(' ');
  return s.substr(space + 1, s.length());
}

} // namespace str
