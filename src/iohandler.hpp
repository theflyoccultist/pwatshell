#pragma once

#include <functional>
#include <string>
#include <vector>

namespace IOHandler {

struct RedirectInfo {
    std::vector<std::string> cmdArgs;
    std::string filename;
    int targetFd = 1; // stdout by default
    bool append = false;
};

void redirect(const RedirectInfo &info,
              const std::function<void(const std::vector<std::string> &)> &executeCallback);

} // namespace IOHandler
