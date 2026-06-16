#pragma once

#include "paths.hpp"
#include "pipeline.hpp"
#include <string>
#include <vector>

class Shell {
  public:
    void executePipeline(PipelinePlan &plan, bool &running);

    void echo(const std::vector<std::string> &args);
    void cat(std::string &command);
    void type(std::string &command);
    void pwd();
    void cd(std::string &command);
    int executable(std::vector<std::string> &args);

  private:
    Paths paths;
};
