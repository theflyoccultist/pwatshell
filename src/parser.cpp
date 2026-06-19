#include "parser.hpp"
#include <string>
#include <unistd.h>
#include <unordered_set>
#include <iostream>
#include <termios.h>
#include <vector>
#include "autocomplete.hpp"
#include "str.hpp"

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
            // executable completion
            auto match = autocomplete.match(usrInput);

            if (match.empty() || usrInput.empty()) {
                tabCount = 0;
                //   it is the bell character.
                std::cout << "\x07" << std::flush;
            }

            if (!usrInput.empty()) {
                // single completion (executable)
                if (tabCount == 1 && match.size() == 1) {
                    // clear the current line
                    std::cout << "\033[" << usrInput.length() << "D\033[K" << std::flush;
                    usrInput.clear();

                    for (char i : match[0].filename) {
                        usrInput.push_back(i);
                        std::cout << i << std::flush;
                    }

                    std::cout << ' ' << std::flush;
                    usrInput.push_back(' ');
                    continue;
                }

                // partial completion (executable)
                if (tabCount == 1 && match.size() > 1) {
                    std::string lcp = autocomplete.lcp(match);

                    if (lcp.length() > usrInput.length()) {
                        tabCount = 0;
                        std::cout << "\033[" << usrInput.length() << "D\033[K" << std::flush;
                        usrInput = lcp;
                        std::cout << usrInput << std::flush;
                    } else {
                        std::cout << "\x07" << std::flush;
                    }

                    continue;
                }

                // multiple completions (executable)
                if (tabCount == 2 && match.size() > 1) {
                    tabCount = 0;
                    std::cout << "\r\n";

                    for (const auto &[str, _] : match) {
                        std::cout << str << "  ";
                    }

                    std::cout << "\r\n$ " << usrInput << std::flush;

                    continue;
                }

                // filename completion
                auto tokens = str::tokenize(usrInput);
                std::string argument = "";

                bool endsWithSpace = (!usrInput.empty() && usrInput.back() == ' ');

                if (endsWithSpace) {
                    argument = "";
                } else if (!tokens.empty()) {
                    argument = tokens.back();
                }
                auto fileMatches = autocomplete.matchFilesInDirectory(argument);

                if (fileMatches.empty()) {
                    std::cout << "\x07" << std::flush;
                    continue;
                }

                if (fileMatches.size() == 1) {
                    const auto &candidate = fileMatches[0];

                    // single completion (file or directory)
                    if (!argument.empty()) {
                        std::cout << "\033[" << argument.length() << "D\033[K" << std::flush;

                        size_t index = usrInput.rfind(argument);
                        if (index != std::string::npos) {
                            usrInput.resize(index);
                        }
                    }

                    for (char i : candidate.filename) {
                        usrInput.push_back(i);
                        std::cout << i << std::flush;
                    }

                    if (!candidate.isDirectory) {
                        std::cout << ' ' << std::flush;
                        usrInput.push_back(' ');
                    } else {
                        std::cout << '/' << std::flush;
                        usrInput.push_back('/');
                    }
                } else {
                    // multiple choices logic do do
                }

                continue;
            }
            continue;
        }

        else if (c == BACKSPACE) {
            tabCount = 0;
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
