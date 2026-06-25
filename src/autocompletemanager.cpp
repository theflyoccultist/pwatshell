#include "fileinfo.hpp"
#include "str.hpp"
#include <cctype>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>
#include "autocompletemanager.hpp"

void AutoCompleteManager::completion(std::string &usrInput) {
    if (usrInput.empty()) {
        std::cout << "\x07" << std::flush;
        return;
    }

    if (usrInput != context.lastInput) {
        context.state = AutoCompleteState::IDLE;
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

        switch (context.state) {
        case AutoCompleteState::IDLE:
            this->execLCP(matchingExecutables, usrInput);
            context.state = AutoCompleteState::LCP_COMPLETED;
            break;
        case AutoCompleteState::LCP_COMPLETED:
            this->execMultipleMatches(matchingExecutables, usrInput);
            context.state = AutoCompleteState::LIST_SHOWN;
            break;
        case AutoCompleteState::LIST_SHOWN:
            std::cout << "\x07" << std::flush;
            break;
        default:
            break;
        }
    } else {
        std::string fileName = endsWithSpace ? "" : tokenized.back();
        std::string absolutePath = autocomplete.getAbsolutePath() + '/' + fileName;
        const auto matchingFiles = autocomplete.fileMatch(absolutePath);

        switch (context.state) {
        case AutoCompleteState::IDLE:
            this->fileLCP(matchingFiles, usrInput, fileName);
            context.state = AutoCompleteState::LCP_COMPLETED;
            break;
        case AutoCompleteState::LCP_COMPLETED:
            this->fileMultipleMatches(matchingFiles, usrInput);
            context.state = AutoCompleteState::LIST_SHOWN;
            break;
        case AutoCompleteState::LIST_SHOWN:
            this->nestedFileCompletion(matchingFiles, usrInput);
            context.state = AutoCompleteState::CYCLING_NESTED;
            break;
        case AutoCompleteState::CYCLING_NESTED:
            context.state = AutoCompleteState::IDLE;
            break;
        }
    }

    context.lastInput = usrInput;
}

void AutoCompleteManager::execLCP(const std::vector<FileInfo> &matchingExecutables,
                                  std::string &usrInput) {
    std::string lcp = autocomplete.lcp(matchingExecutables);

    if (lcp.length() > usrInput.length()) {
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
}

void AutoCompleteManager::execMultipleMatches(const std::vector<FileInfo> &matchingExecutables,
                                              const std::string &usrInput) {
    if (matchingExecutables.empty()) {
        std::cout << "\x07" << std::flush;
        return;
    }

    std::cout << "\r\n";

    for (const auto &[str, _] : matchingExecutables) {
        std::cout << str << "  ";
    }

    std::cout << "\r\n$ " << usrInput << std::flush;
}

void AutoCompleteManager::fileLCP(const std::vector<FileInfo> &matchingFiles, std::string &usrInput,
                                  const std::string &fileName) {
    std::string lcp = autocomplete.lcp(matchingFiles);

    if (lcp.length() > usrInput.length()) {
        std::cout << "\033[" << usrInput.length() << "D\033[K" << std::flush;
        str::eraseCommonSubString(lcp, autocomplete.getAbsolutePath() + '/');

        size_t index = usrInput.rfind(fileName);
        if (index != std::string::npos) {
            usrInput.resize(index);
        }

        usrInput += lcp;
        std::cout << usrInput << std::flush;

        if (matchingFiles.size() == 1) {
            if (matchingFiles[0].isDirectory) {
                std::cout << '/' << std::flush;
                usrInput.push_back('/');
            } else {
                std::cout << ' ' << std::flush;
                usrInput.push_back(' ');
            }
        }
    } else {
        std::cout << "\x07" << std::flush;
    }
}

void AutoCompleteManager::fileMultipleMatches(const std::vector<FileInfo> &matchingFiles,
                                              const std::string &usrInput) {
    if (matchingFiles.empty()) {
        std::cout << "\x07" << std::flush;
        return;
    }
    std::cout << "\r\n";

    for (auto [str, _] : matchingFiles) {
        str::eraseCommonSubString(str, autocomplete.getAbsolutePath() + '/');
        std::cout << str << "  ";
    }

    std::cout << "\r\n$ " << usrInput << std::flush;
}

void AutoCompleteManager::nestedFileCompletion(const std::vector<FileInfo> &matchingFiles,
                                               std::string &usrInput) {
    if (matchingFiles.empty()) {
        std::cout << "\x07" << std::flush;
        return;
    }
    //
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
