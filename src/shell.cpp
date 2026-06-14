#include "shell.hpp"
#include "opts.hpp"
#include "paths.hpp"
#include "str.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <vector>

void Shell::echo(const std::vector<std::string> &args) {
    for (size_t i = 1; i < args.size(); ++i) {
        std::cout << args[i];
        if (i + 1 < args.size()) {
            std::cout << " ";
        }
    }
    std::cout << "\n";
}

void Shell::cat(std::string &command) { ::system(command.c_str()); }

void Shell::type(std::string &command) {
    std::string userInput = str::rtrim(command);

    if (opts::resolveOption(userInput) != Options::Executable &&
        opts::resolveOption(userInput) != Options::Cat) {
        std::cout << userInput << " is a shell builtin\n";
    } else if (paths.getExecutablePath(userInput) != "") {
        std::cout << userInput << " is " << paths.getExecutablePath(userInput).string() << "\n";
    } else {
        std::cout << userInput << ": not found\n";
    }
}

void Shell::pwd() { std::cout << paths.pwd() << "\n"; }

void Shell::cd(std::string &command) {
    std::string userInput = str::rtrim(command);
    paths.changeDirectory(userInput);
}

int Shell::executable(std::vector<std::string> &args) {
    std::vector<char *> argv;

    argv.reserve(args.size() + 1);

    for (auto &arg : args) {
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
