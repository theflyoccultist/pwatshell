#pragma once

#include <string>
#include <vector>

#ifdef _WIN32
constexpr char PATH_LIST_SEPARATOR = ';';
#else
constexpr char PATH_LIST_SEPARATOR = ':';
#endif

namespace str {
std::string ltrim(std::string &s);

std::string rtrim(std::string &s);

std::string getPath();

std::vector<std::string> splitPath(const std::string &path);
} // namespace str
