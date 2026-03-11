#pragma once

#include <string>

class Shell {
public:
  static void echo(std::string &command);
  static void type(std::string &command);
  static void pwd();
  static void cd(std::string &command);
  static int executable(std::string &command);
};
