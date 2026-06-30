#include "shell.hpp"
#include "opts.hpp"
#include "paths.hpp"
#include "iohandler.hpp"
#include "pipeline.hpp"
#include "history.hpp"

#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

void Shell::run(const PipelinePlan &plan) {
    if (plan.commands.empty())
        return;

    if (plan.commands.size() == 1) {
        const std::string &cmd_name = plan.commands[0].args[0];

        if (opts::resolveOption(cmd_name) != Options::Executable) {
            IOHandler::RedirectInfo info = {
                .cmdArgs = plan.commands[0].args,
                .filename = plan.redirectFilename,
                .targetFd = plan.targetFd,
                .isAppend = plan.isAppend,
            };

            if (plan.hasRedirect) {
                IOHandler::redirect(info, [this](const std::vector<std::string> &execArgs) {
                    this->executeCommand(execArgs);
                });
            } else {
                this->executeCommand(info.cmdArgs);
            }

            return;
        }
    }

    this->executePipeline(plan);
}

void Shell::executePipeline(const PipelinePlan &plan) {
    pid_t cpid = 0;
    int status = 0;
    std::array<int, 2> pipefd{};
    char buf = 0;
    int in_fd = STDIN_FILENO;

    for (size_t i = 0; i < plan.commands.size(); ++i) {
        bool pipe_opened = false;
        if (i < plan.commands.size() - 1) {
            if (pipe(pipefd.data()) == -1) {
                perror("pipe");
            }
            pipe_opened = true;
        }

        cpid = fork();
        if (cpid == -1) {
            perror("fork");
        }

        if (cpid == 0) {
            if (dup2(in_fd, STDIN_FILENO) == -1) {
                perror("dup2 child - read");
            }

            if (i < plan.commands.size() - 1) {
                if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
                    perror("dup2 child - write");
                }
            }

            close(pipefd[0]);

            IOHandler::RedirectInfo info = {
                .cmdArgs = plan.commands[i].args,
                .filename = plan.redirectFilename,
                .targetFd = plan.targetFd,
                .isAppend = plan.isAppend,
            };

            if (plan.hasRedirect) {
                IOHandler::redirect(info, [this](const std::vector<std::string> &execArgs) {
                    this->executeCommand(execArgs);
                });
            } else {
                this->executeCommand(info.cmdArgs);
            }

            exit(EXIT_FAILURE);

        } else {
            if (in_fd != STDIN_FILENO) {
                close(in_fd);
            }

            if (pipe_opened) {
                close(pipefd[1]);
                in_fd = pipefd[0];
            }
        }
    }

    for (size_t i = 0; i < plan.commands.size(); ++i) {
        if (wait(nullptr) == -1) {
            perror("wait");
        }
    }
}

int Shell::commandcounter{};

void Shell::executeCommand(const std::vector<std::string> &args) {
    if (args.empty())
        return;

    commandcounter++;
    Options opts = opts::resolveOption(args[0]);

    switch (opts) {
    case Options::Echo:
        this->echo(args);
        break;
    case Options::Type:
        this->type(args);
        break;
    case Options::Pwd:
        this->pwd();
        break;
    case Options::History:
        this->historyCmd(commandcounter, args);
        break;
    case Options::Cd:
        this->cd(args);
        break;
    case Options::Exit:
        break;
    case Options::Executable:
        this->executable(args);
        break;
    }
}

void Shell::echo(const std::vector<std::string> &args) const {
    for (size_t i = 1; i < args.size(); ++i) {
        std::cout << args[i];
        if (i + 1 < args.size()) {
            std::cout << " ";
        }
    }
    std::cout << "\n";
}

void Shell::type(const std::vector<std::string> &args) const {
    if (opts::resolveOption(args[1]) != Options::Executable) {
        std::cout << args[1] << " is a shell builtin\n";
    } else if (paths.getExecutablePath(args[1]) != "") {
        std::cout << args[1] << " is " << paths.getExecutablePath(args[1]).string() << "\n";
    } else {
        std::cout << args[1] << ": not found\n";
    }
}

void Shell::pwd() const { std::cout << paths.pwd() << "\n"; }

void Shell::historyCmd(int &numcmds, const std::vector<std::string> &args) const {
    if (args.size() <= 1) {
        History::listHistory();
    } else if (args.size() == 2) {
        try {
            int value = std::stoi(args[1]);
            if (value < 0) {
                std::cout << "history: " << value << ": invalid option\n";
            } else {
                History::listHistory(value);
            }

        } catch (const std::invalid_argument &e) {
            std::cout << "history: " << args[1] << ": numeric argument required\n";
        }
    } else if (args.size() > 2) {
        History::parseHistoryFlag(numcmds, args);
    }
}

void Shell::cd(const std::vector<std::string> &args) const {
    if (args.size() <= 1) {
        paths.changeDirectory(paths.getPathHome());
    } else {
        paths.changeDirectory(args[1]);
    }
}

void Shell::executable(const std::vector<std::string> &args) const {
    std::vector<std::string> args_mutable = args;
    std::vector<char *> argv;

    argv.reserve(args_mutable.size() + 1);

    for (auto &arg : args_mutable) {
        argv.push_back(arg.data());
    }

    argv.push_back(nullptr);

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        ::execvp(argv[0], argv.data());
        std::cout << argv[0] << ": command not found\n";
        exit(EXIT_FAILURE);
    } else {
        ::waitpid(pid, nullptr, 0);
        exit(EXIT_SUCCESS);
    }
}
