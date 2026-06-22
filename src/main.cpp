#include "parser.hpp"
#include "pipeline.hpp"
#include "shell.hpp"
#include "str.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

int main() {
    Shell shell;
    Parser parser;
    bool running = true;

    // Flush after every std::cout / std:cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    while (running) {
        std::cout << "$ ";
        std::string inputLine = parser.parseUsrInput();
        if (inputLine.empty())
            continue;

        auto tokens = str::tokenize(inputLine);

        PipelinePlan plan = parser.parse(tokens);

        shell.executePipeline(plan, running);
    }
}
