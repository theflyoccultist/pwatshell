#pragma once

#include <string>
#include <vector>

class Shell {
public:
  static void echo(std::string &command);
  static void type(std::string &command);
  static void invalid(std::string &command);
  static std::vector<char *> executableCmds(const std::string &command);
  static int executable(std::string &command);
};
