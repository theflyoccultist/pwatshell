#pragma once

#include "paths.hpp"
#include <string>

class Shell {
public:
  Shell(Paths &ph) : paths(ph) {}

  void echo(std::string &command);
  void type(std::string &command);
  void invalid(std::string &command);

private:
  Paths &paths;
};
