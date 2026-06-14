#pragma once

#include "paths.hpp"
#include <string>
#include <vector>

class Shell {
  public:
    void echo(const std::vector<std::string> &command);
    void cat(std::string &command);
    void type(std::string &command);
    void pwd();
    void cd(std::string &command);
    int executable(std::string &command);

  private:
    Paths paths;
};
