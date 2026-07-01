#pragma once

#include "history.hpp"
#include "paths.hpp"
#include "pipeline.hpp"
#include "shellstate.hpp"
#include <string>
#include <vector>

class Shell {
  public:
    void run(const PipelinePlan &plan);

  private:
    void executePipeline(const PipelinePlan &plan);

    void echo(const std::vector<std::string> &args) const;
    void type(const std::vector<std::string> &args) const;
    void pwd() const;
    void historyCmd(const std::vector<std::string> &args);
    void jobsCmd(const std::vector<std::string> &args);
    void cd(const std::vector<std::string> &args) const;
    void executable(const std::vector<std::string> &args) const;

    Paths paths;
    History history;
    ShellState state;

    void executeCommand(const std::vector<std::string> &args);
};
