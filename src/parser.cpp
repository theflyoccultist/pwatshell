#include "parser.hpp"
#include <string>
#include <unistd.h>
#include <unordered_set>
#include <iostream>
#include <termios.h>
#include "autocomplete.hpp"

namespace Parser {

constexpr size_t MAX_LINE_LEN = 4096;
constexpr char BACKSPACE = 127;

const AutoComplete autocomplete;

std::string parseUsrInput() {
    std::string usrInput{};
    usrInput.reserve(MAX_LINE_LEN);

    termios term_canonical{}, term_raw{};

    if (tcgetattr(STDIN_FILENO, &term_canonical) == -1) {
        perror("tcgetattr error");
        return "";
    }

    term_raw = term_canonical;
    cfmakeraw(&term_raw);

    // Apply the raw settings to the terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &term_raw);

    char c{};
    int tabCount = 0;
    while (true) {
        ssize_t bytesRead = read(STDIN_FILENO, &c, 1);
        if (bytesRead <= 0)
            break;

        // In raw mode Enter is '\r' (carriage return)
        if (c == '\r' || c == '\n') {
            std::cout << "\r\n";
            break;
        } else if (c == '\t') {
            tabCount++;
            auto match = autocomplete.match(usrInput);

            if (match.empty() || usrInput.empty()) {
                std::cout << "\x07" << std::flush;
            }

            if (!usrInput.empty()) {
                if (tabCount == 1 && match.size() == 1) {
                    // clear the current line
                    std::cout << "\033[" << usrInput.length() << "D\033[K" << std::flush;
                    usrInput.clear();

                    for (const std::string &str : match) {
                        for (const char &c : str) {
                            usrInput.push_back(c);
                            std::cout << c << std::flush;
                        }
                        std::cout << ' ' << std::flush;
                        usrInput.push_back(' ');
                    }
                } else if (tabCount == 1 && match.size() > 1) {
                    // it is the bell character.
                    std::cout << "\x07" << std::flush;
                }

                if (tabCount == 2 && match.size() > 1) {
                    tabCount = 0;
                    std::cout << "\r\n";

                    for (const std::string &str : match) {
                        std::cout << str << "  ";
                    }

                    std::cout << "\r\n$ " << usrInput << std::flush;

                    continue;
                }
            }

            continue;
        }

        else if (c == BACKSPACE) {
            if (!usrInput.empty()) {
                usrInput.pop_back();
                std::cout << "\b \b" << std::flush;
            }
            continue;
        }

        std::cout << c << std::flush;
        usrInput.push_back(c);
    }

    // Revert to canonical settings
    tcsetattr(STDIN_FILENO, TCSANOW, &term_canonical);

    return usrInput;
}

const std::unordered_set<std::string> redirects = {">", "1>", "2>", ">>", "1>>", "2>>"};

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

        else if (redirects.find(token) != redirects.end()) {
            // the next token must be the filename
            if (i + 1 < tokens.size()) {
                plan.hasRedirect = true;
                plan.redirectFilename = tokens[i + 1];
                plan.isAppend = (token == ">>") || (token == "1>>") || (token == "2>>");
                plan.targetFd = (token == "2>") || (token == "2>>") ? 2 : 1;

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
