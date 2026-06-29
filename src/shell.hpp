#pragma once

#include "paths.hpp"
#include "pipeline.hpp"
#include <string>
#include <vector>

class Shell {
  public:
    void executePipeline(const PipelinePlan &plan) const;

    void echo(const std::vector<std::string> &args) const;
    void type(const std::vector<std::string> &args) const;
    void pwd() const;
    void cd(const std::vector<std::string> &args) const;
    [[nodiscard]] int executable(const std::vector<std::string> &args) const;

  private:
    Paths paths;

    void executeCommand(const std::vector<std::string> &args) const;
};
