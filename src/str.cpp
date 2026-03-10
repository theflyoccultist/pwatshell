#include "str.hpp"

#include <sstream>

namespace str {
std::string ltrim(std::string &s) {
  int space = s.find(' ');
  return s.substr(0, space);
}

std::string rtrim(std::string &s) {
  int space = s.find(' ');
  return s.substr(space + 1, s.length());
}

std::string getPath() {
  const char *path = std::getenv("PATH");
  std::string path_str;

  if (path) {
    path_str = path;
  } else {
    path_str = "";
  }

  return path_str;
}

std::vector<std::string> splitPath(const std::string &path) {
  std::vector<std::string> dirs;
  std::stringstream ss(path);
  std::string item;

  while (std::getline(ss, item, PATH_LIST_SEPARATOR)) {
    dirs.push_back(item);
  }

  return dirs;
}

} // namespace str
