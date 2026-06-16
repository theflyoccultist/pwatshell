#pragma once

#include <vector>
#include <string>

struct Command {
    std::vector<std::string> args;
};

struct PipelinePlan {
    std::vector<Command> commands;

    // Redirection info for the overall pipeline
    std::string redirectFilename;
    int targetFd = 1; // 1 for stdout, 2 for stderr
    bool isAppend = false;
    bool hasRedirect = false;
};
