#pragma once

#include "paths.hpp"
#include "pipeline.hpp"
#include <string>
#include <vector>

class Shell {
  public:
    void executePipeline(const PipelinePlan &plan, bool &running);

    void echo(const std::vector<std::string> &args);
    void type(const std::vector<std::string> &args);
    void pwd();
    void cd(const std::vector<std::string> &args);
    int executable(const std::vector<std::string> &args);

  private:
    Paths paths;

    void executeCommand(const std::vector<std::string> &args, bool &running);
};
