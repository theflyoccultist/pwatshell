#pragma once

#include <string>
#include <vector>

namespace str {
std::vector<std::string> tokenize(const std::string &input);

std::string ltrim(const std::string &s);
std::string rtrim(const std::string &s);

std::vector<std::string> splitString(const std::string &input, char delimiter);
std::string concatString(const std::vector<std::string> &vecStr, char delimiter);
} // namespace str
