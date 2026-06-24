#include "fileinfo.hpp"
#include "str.hpp"
#include <cctype>
#include <cstddef>
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

    bool endsWithSpace = (!usrInput.empty() && std::isspace(usrInput.back()));

    size_t completionIndex = tokenized.size();
    if (!endsWithSpace && completionIndex > 0) {
        completionIndex--;
    }

    if (completionIndex == 0) {
        const std::string &execName = endsWithSpace ? "" : tokenized.front();
        const auto matchingExecutables = autocomplete.execMatch(execName);
        this->execCompletion(tabCount, matchingExecutables, usrInput);
    } else {
        std::string fileName = endsWithSpace ? "" : tokenized.back();
        std::string absolutePath = autocomplete.getAbsolutePath() + '/' + fileName;
        const auto matchingFiles = autocomplete.fileMatch(absolutePath);

        this->fileCompletion(tabCount, matchingFiles, usrInput, fileName);
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

void AutoCompleteManager::fileCompletion(int &tabCount, const std::vector<FileInfo> &matchingFiles,
                                         std::string &usrInput, const std::string &fileName) {
    if (matchingFiles.empty()) {
        tabCount = 0;
        std::cout << "\x07" << std::flush;
        return;
    }

    if (matchingFiles.size() == 1) {
        std::cout << "\033[" << fileName.length() << "D\033[K" << std::flush;

        size_t index = usrInput.rfind(fileName);
        if (index != std::string::npos) {
            usrInput.resize(index);
        }

        const FileInfo &match = matchingFiles[0];
        std::string printMatch = match.filename;
        str::eraseCommonSubString(printMatch, autocomplete.getAbsolutePath() + '/');

        for (char i : printMatch) {
            usrInput.push_back(i);
            std::cout << i << std::flush;
        }

        if (match.isDirectory) {
            std::cout << '/' << std::flush;
            usrInput.push_back('/');
        } else {
            std::cout << ' ' << std::flush;
            usrInput.push_back(' ');
        }

        return;
    }

    if (matchingFiles.size() > 1) {
        // if (tabCount == 1) {
        //     // lcp to do
        //     return;
        // }

        if (tabCount == 2) {
            tabCount = 0;
            std::cout << "\r\n";

            for (auto [str, _] : matchingFiles) {
                str::eraseCommonSubString(str, autocomplete.getAbsolutePath() + '/');
                std::cout << str << "  ";
            }

            std::cout << "\r\n$ " << usrInput << std::flush;
            return;
        }
    }
}

void AutoCompleteManager::nestedFileCompletion(const std::vector<FileInfo> &matchingFiles,
                                               std::string &usrInput) {
    // autocomplete.refreshFilesTrie(match.filename + '/');
    //
    // size_t countDir = 0;
    // size_t dirPos = 0;
    // for (size_t i = 0; i < filesInNestedPath.size(); ++i) {
    //     if (filesInNestedPath[i].isDirectory) {
    //         countDir++;
    //         dirPos = i;
    //     }
    // }
    //
    // if (countDir == 1) {
    //     std::cout << "\r\nmatching directory: " << filesInNestedPath[dirPos].filename << "\r\n"
    //               << std::flush;
    //     usrInput += filesInNestedPath[dirPos].filename;
    // }
}
