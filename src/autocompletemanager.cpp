#include "fileinfo.hpp"
#include "str.hpp"
#include <cctype>
#include <cstddef>
#include <iostream>
#include <string>
#include "autocompletemanager.hpp"

void AutoCompleteManager::completion(int &tabCount, std::string &usrInput) {
    const auto tokenized = str::tokenize(usrInput);

    bool endsWithSpace = (!usrInput.empty() && std::isspace(usrInput.back()));

    size_t completionIndex = tokenized.size();
    if (!endsWithSpace && completionIndex > 0) {
        completionIndex--;
    }

    if (completionIndex == 0) {
        const std::string &execName = endsWithSpace ? "" : tokenized.front();
        const auto matchingExecutables = autocomplete.execMatch(execName);
        this->execCompletion(tabCount, matchingExecutables, usrInput);
    }
}

void AutoCompleteManager::execCompletion(int &tabCount,
                                         const std::vector<FileInfo> &matchingExecutables,
                                         std::string &usrInput) {
    if (matchingExecutables.empty()) {
        tabCount = 0;
        std::cout << "\x07" << std::flush;
        return;
    }

    if (tabCount == 1) {
        std::string lcp = autocomplete.lcp(matchingExecutables);

        if (lcp.length() > usrInput.length()) {
            tabCount = 0;
            std::cout << "\033[" << usrInput.length() << "D\033[K" << std::flush;
            usrInput = lcp;
            std::cout << usrInput << std::flush;

            if (matchingExecutables.size() == 1) {
                std::cout << ' ' << std::flush;
                usrInput.push_back(' ');
            }
        } else {
            std::cout << "\x07" << std::flush;
        }
        return;
    }

    if (tabCount == 2) {
        tabCount = 0;
        std::cout << "\r\n";

        for (const auto &[str, _] : matchingExecutables) {
            std::cout << str << "  ";
        }

        std::cout << "\r\n$ " << usrInput << std::flush;
        return;
    }
}
