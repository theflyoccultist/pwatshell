#pragma once

#include "paths.hpp"
#include <string>

class Shell {
public:
  void echo(std::string &command) const;
  void type(std::string &command) const;
  void pwd() const;
  void cd(std::string &command);
  int executable(std::string &command) const;

private:
  Paths paths;
};
