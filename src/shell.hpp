#pragma once

#include <string>

#ifdef _WIN32
constexpr char PATH_LIST_SEPARATOR[] = ";";
#else
constexpr char PATH_LIST_SEPARATOR[] = ":";
#endif

namespace shell {
void echo(std::string &command);
void type(std::string &command);
void invalid(std::string &command);
} // namespace shell
