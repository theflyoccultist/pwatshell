#include "str.hpp"
#include <cstddef>
#include <sstream>
#include <vector>

namespace str {
std::string ltrim(const std::string &s) {
    std::size_t space = s.find(' ');
    return s.substr(0, space);
}

std::string rtrim(const std::string &s) {
    std::size_t space = s.find(' ');
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

std::string concatString(const std::vector<std::string> &vecStr) {
    std::string concat;

    for (auto &str : vecStr) {
        concat += str;
    }

    return concat;
}

} // namespace str
