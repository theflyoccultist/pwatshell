#include <algorithm>
#include <iterator>
#include <vector>
#include "autocomplete.hpp"

AutoComplete::AutoComplete() {
    this->initExecutableList();

    for (const auto &exe : executableList) {
        trieForExecutables.insertWord(exe);
    }
}

std::vector<std::string> AutoComplete::match(const std::string &usrInput) const {
    return trieForExecutables.getSuggestions(usrInput);
}

std::string AutoComplete::lcp(const std::vector<std::string> &words) const {
    if (words.empty()) {
        return "";
    }

    if (words.size() == 1) {
        return words[0];
    }

    for (size_t charIndex = 0; charIndex < words[0].size(); ++charIndex) {
        for (size_t stringIndex = 1; stringIndex < words.size(); ++stringIndex) {
            if (words[stringIndex].size() <= charIndex ||
                words[stringIndex][charIndex] != words[0][charIndex]) {
                return words[0].substr(0, charIndex);
            }
        }
    }

    return words[0];
}

std::vector<std::string> AutoComplete::matchFilesInDirectory(const std::string &usrInput) const {
    const std::vector<std::string> &fileNames = paths.getFilesInCurrPath();

    for (const auto &file : fileNames) {
        trieForFiles.insertWord(file);
    }

    return trieForFiles.getSuggestions(usrInput);
}

void AutoComplete::initExecutableList() {
    executableList = {"echo", "type", "pwd", "cd", "exit"};

    const std::vector<std::string> &execNames = paths.getExecutablesInPathEnv();

    std::ranges::move(execNames, std::back_inserter(executableList));
}
