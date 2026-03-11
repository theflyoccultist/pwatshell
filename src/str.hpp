#pragma once

#include <string>
#include <vector>

namespace str {
std::string ltrim(std::string &s);

std::string rtrim(std::string &s);
std::vector<std::string> splitString(const std::string &input, char delimiter);
} // namespace str
