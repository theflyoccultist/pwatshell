#include "iohandler.hpp"
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>

namespace IOHandler {

void redirect(const RedirectInfo &info,
              const std::function<void(const std::vector<std::string> &)> &executeCallback) {
    // Back up original stdout
    int saved_fd = dup(info.targetFd);

    if (saved_fd == -1) {
        perror("dup error");
        return;
    }

    // should create file if it doesn't exist, overwrite if it exists
    int oflag = O_WRONLY | O_CREAT | (info.isAppend ? O_APPEND : O_TRUNC);
    mode_t perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
    int file_fd = open(info.filename.c_str(), oflag, perms);

    if (file_fd == -1) {
        perror("open error");
        close(saved_fd);
        return;
    }

    // redirection
    dup2(file_fd, info.targetFd);
    close(file_fd);

    // Run the command
    executeCallback(info.cmdArgs);

    // restore original stdout
    dup2(saved_fd, info.targetFd);
    close(saved_fd);
}

} // namespace IOHandler
