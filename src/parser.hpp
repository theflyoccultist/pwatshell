#pragma once

#include "pipeline.hpp"
#include <iostream>
#include <string>
#include <vector>

namespace Parser {

PipelinePlan parse(const std::vector<std::string> &tokens) {
    PipelinePlan plan;
    Command currentCmd;

    for (size_t i = 0; i < tokens.size(); ++i) {
        const std::string &token = tokens[i];

        if (token == "|") {
            // Pipe: push the current command to the plan and start a new one
            if (!currentCmd.args.empty()) {
                plan.commands.push_back(currentCmd);
                currentCmd.args.clear();
            }
        }

        else if (token == ">" || token == ">>" || token == "1>" || token == "2>") {
            // the next token must be the filename
            if (i + 1 < tokens.size()) {
                plan.hasRedirect = true;
                plan.redirectFilename = tokens[i + 1];
                plan.isAppend = (token == ">>");
                plan.targetFd = (token == "2>") ? 2 : 1;

                // Skip the next token, since we just consumed it as the filename
                i++;
            } else {
                std::cerr << "Shell syntax error: Missing filename after redirect\n";
            }
        } else {
            // regular argument / executable name
            currentCmd.args.push_back(token);
        }
    }

    // Push the final command left in the buffer
    if (!currentCmd.args.empty()) {
        plan.commands.push_back(currentCmd);
    }

    return plan;
}

} // namespace Parser
