#include "str.hpp"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>

namespace str {

enum class State : ::uint8_t { Normal, Escaping, InSingleQuotes, InDoubleQuotes, Redirect };

std::vector<std::string> tokenize(const std::string &input) {
    std::vector<std::string> tokens;
    std::string currentToken;
    State state = State::Normal;

    for (size_t i = 0; i < input.length(); ++i) {
        char c = input[i];

        switch (state) {
        case State::Normal:
            if (c == '\\') {
                state = State::Escaping;
            } else if (c == '\'') {
                state = State::InSingleQuotes;
            } else if (c == '"') {
                state = State::InDoubleQuotes;
            } else if (c == '>') {
                currentToken.push_back(c);
                state = State::Redirect;
            } else if (c == ' ') {
                if (!currentToken.empty()) {
                    tokens.push_back(currentToken);
                    currentToken.clear();
                }
            } else {
                currentToken.push_back(c);
            }
            break;

        case State::Escaping:
            currentToken.push_back(c);
            state = State::Normal;
            break;

        case State::InSingleQuotes:
            if (c == '\'') {
                state = State::Normal;
            } else {
                currentToken.push_back(c);
            }
            break;

        case State::InDoubleQuotes:
            if (c == '"') {
                state = State::Normal;
            } else if (c == '\\') {
                currentToken.push_back(input[++i]);
            } else {
                currentToken.push_back(c);
            }
            break;

        case State::Redirect:
            if (input[i] == '>') {
                currentToken.push_back(c);
            }
            tokens.push_back(currentToken);
            currentToken.clear();
            state = State::Normal;
            break;
        }
    }

    if (!currentToken.empty()) {
        tokens.push_back(currentToken);
    }

    if (state != State::Normal) {
        std::cerr << "Shell error: Unterminated Quote\n";
        return {};
    }

    return tokens;
}

std::string ltrim(const std::string &s) {
    std::size_t space = s.find(' ');
    return s.substr(0, space);
}

std::string rtrim(const std::string &s) {
    std::size_t space = s.find(' ');
    return s.substr(space + 1, s.length());
}

std::vector<std::string> splitString(const std::string &input, char delimiter) {
    std::vector<std::string> dirs;
    std::stringstream ss(input);
    std::string item;

    while (std::getline(ss, item, delimiter)) {
        dirs.push_back(item);
    }

    return dirs;
}

std::string concatString(const std::vector<std::string> &vecStr, char delimiter) {
    std::string concat;

    for (auto &str : vecStr) {
        concat += (delimiter + str);
    }

    return concat;
}

} // namespace str
