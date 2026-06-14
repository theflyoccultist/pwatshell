#include "shell.hpp"
#include "opts.hpp"
#include "paths.hpp"
#include "str.hpp"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <vector>

void Shell::echo(std::string &command) {
    std::string trimmedCmd = str::rtrim(command);
    const auto &split = str::splitString(trimmedCmd, '\'');

    if (split.size() > 1) {
        const std::string &concat = str::concatString(split);
        std::cout << concat << '\n';
    } else {
        const auto ret =
            std::ranges::unique(trimmedCmd, [](char a, char b) { return a == ' ' && b == ' '; });
        trimmedCmd.erase(ret.begin(), ret.end());
        std::cout << trimmedCmd << '\n';
    }
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

int Shell::executable(std::string &command) {
    std::vector<std::string> args = str::splitString(command, ' ');
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
