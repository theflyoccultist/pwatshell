#include "opts.hpp"
#include "shell.hpp"
#include "str.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

int main() {
    bool running = true;
    Shell shell;

    while (running) {
        // Flush after every std::cout / std:cerr
        std::cout << std::unitbuf;
        std::cerr << std::unitbuf;

        std::cout << "$ ";
        std::string command;
        std::getline(std::cin, command);

        Options opts = opts::resolveOption(str::ltrim(command));
        auto tokens = str::tokenize(command);

        switch (opts) {
        case Options::Echo: {
            shell.echo(tokens);
        } break;
        case Options::Type:
            shell.type(command);
            break;
        case Options::Pwd:
            shell.pwd();
            break;
        case Options::Cd:
            shell.cd(command);
            break;
        case Options::Cat:
            shell.cat(command);
            break;
        case Options::Exit:
            running = false;
            break;
        case Options::Executable:
            shell.executable(tokens);
            break;
        }
    }
}
