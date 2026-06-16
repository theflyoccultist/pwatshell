#include "shell.hpp"
#include "opts.hpp"
#include "paths.hpp"
#include "iohandler.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <vector>

void Shell::executePipeline(const PipelinePlan &plan, bool &running) {
    IOHandler::RedirectInfo info = {
        .cmdArgs = plan.commands[0].args, // temporary until i get to pipes
        .filename = plan.redirectFilename,
        .targetFd = plan.targetFd,
        .isAppend = plan.isAppend,
    };

    if (plan.hasRedirect) {
        IOHandler::redirect(info, [this, &running](const std::vector<std::string> &execArgs) {
            this->executeCommand(execArgs, running);
        });
    } else {
        this->executeCommand(plan.commands[0].args, running);
    }
}

void Shell::executeCommand(const std::vector<std::string> &args, bool &running) {
    if (args.empty())
        return;

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
    case Options::Cd:
        this->cd(args);
        break;
    case Options::Exit:
        running = false;
        break;
    case Options::Executable:
        this->executable(args);
        break;
    }
}

void Shell::echo(const std::vector<std::string> &args) {
    for (size_t i = 1; i < args.size(); ++i) {
        std::cout << args[i];
        if (i + 1 < args.size()) {
            std::cout << " ";
        }
    }
    std::cout << "\n";
}

void Shell::type(const std::vector<std::string> &args) {
    if (opts::resolveOption(args[1]) != Options::Executable) {
        std::cout << args[1] << " is a shell builtin\n";
    } else if (paths.getExecutablePath(args[1]) != "") {
        std::cout << args[1] << " is " << paths.getExecutablePath(args[1]) << "\n";
    } else {
        std::cout << args[1] << ": not found\n";
    }
}

void Shell::pwd() { std::cout << paths.pwd() << "\n"; }

void Shell::cd(const std::vector<std::string> &args) { paths.changeDirectory(args[1]); }

int Shell::executable(const std::vector<std::string> &args) {
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
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        ::execvp(argv[0], argv.data());
        std::cout << argv[0] << ": command not found\n";
        exit(EXIT_FAILURE);
    } else {
        ::waitpid(pid, nullptr, 0);
        return EXIT_SUCCESS;
    }
}
