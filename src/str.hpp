#pragma once

#include <string>
#include <vector>

namespace str {
std::vector<std::string> tokenize(const std::string &input);

std::vector<std::string> splitString(const std::string &input, char delimiter);

void eraseCommonSubString(std::string &mainStr, const std::string &subStr);
} // namespace str
