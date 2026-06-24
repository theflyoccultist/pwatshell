#include "fileinfo.hpp"
#include "str.hpp"
#include <iostream>
#include <string>
#include "autocompletemanager.hpp"

void AutoCompleteManager::completion(int &tabCount, std::string &usrInput) {
    if (usrInput.empty()) {
        tabCount = 0;
        std::cout << "\x07" << std::flush;
        return;
    }

    const auto tokenized = str::tokenize(usrInput);

    std::string execName = tokenized.front();
    std::string fileName = tokenized.back();

    // std::cout << "\r\nexecName: " << execName << std::flush;
    // std::cout << "\r\nfileName: " << fileName << std::flush;
    // std::cout << "\r\ntoken size: " << tokenized.size() << std::flush;
    //
    if (tokenized.size() == 1) {
        const auto matchingExecutables = autocomplete.execMatch(execName);
        this->execCompletion(tabCount, matchingExecutables, usrInput);
    }

    const auto matchingFiles = autocomplete.fileMatch(fileName);
    this->fileCompletion(matchingFiles, usrInput, fileName);
}

void AutoCompleteManager::execCompletion(int &tabCount,
                                         const std::vector<FileInfo> &matchingExecutables,
                                         std::string &usrInput) {
    if (matchingExecutables.empty()) {
        tabCount = 0;
        std::cout << "\x07" << std::flush;
        return;
    }

    if (matchingExecutables.size() == 1) {
        std::cout << "\033[" << usrInput.length() << "D\033[K" << std::flush;
        usrInput.clear();

        for (char i : matchingExecutables[0].filename) {
            usrInput.push_back(i);
            std::cout << i << std::flush;
        }

        std::cout << ' ' << std::flush;
        usrInput.push_back(' ');
        return;
    }

    if (matchingExecutables.size() > 1) {
        if (tabCount == 1) {
            std::string lcp = autocomplete.lcp(matchingExecutables);

            if (lcp.length() > usrInput.length()) {
                tabCount = 0;
                std::cout << "\033[" << usrInput.length() << "D\033[K" << std::flush;
                usrInput = lcp;
                std::cout << usrInput << std::flush;
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
}

void AutoCompleteManager::fileCompletion(const std::vector<FileInfo> &matchingFiles,
                                         std::string &usrInput, std::string &fileName) {
    if (matchingFiles.empty()) {
        std::cout << "\x07" << std::flush;
        return;
    }

    if (matchingFiles.size() == 1) {
        std::cout << "\033[" << fileName.length() << "D\033[K" << std::flush;
        fileName.clear();

        for (char i : matchingFiles[0].filename) {
            fileName.push_back(i);
            std::cout << i << std::flush;
        }

        usrInput += fileName;
    }
    std::cout << "\r\n$ " << "metching files more than one" << std::flush;
}
