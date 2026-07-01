#pragma once

#include "history.hpp"
#include "paths.hpp"
#include "pipeline.hpp"
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
    void historyCmd(int &numcmds, const std::vector<std::string> &args);
    void cd(const std::vector<std::string> &args) const;
    void executable(const std::vector<std::string> &args) const;

    Paths paths;
    History history;

    void executeCommand(const std::vector<std::string> &args);
    static int commandcounter;
};
