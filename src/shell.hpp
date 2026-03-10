#pragma once

#include <string>

namespace shell {
void echo(std::string &command);
void type(std::string &command);
void invalid(std::string &command);
} // namespace shell
