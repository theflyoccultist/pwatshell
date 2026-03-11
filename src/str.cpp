#include "str.hpp"
#include <sstream>

namespace str {
std::string ltrim(std::string &s) {
  unsigned long space = s.find(' ');
  return s.substr(0, space);
}

std::string rtrim(std::string &s) {
  unsigned long space = s.find(' ');
  return s.substr(space + 1, s.length());
}

std::vector<std::string> splitString(const std::string &input, char delimiter) {
  std::vector<std::string> dirs;
  std::stringstream ss(input);
  std::string item;

  while (std::getline(ss, item, delimiter)) {
    dirs.push_back(item);
  }

  return dirs;
}

} // namespace str
