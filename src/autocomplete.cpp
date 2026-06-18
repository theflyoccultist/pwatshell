#include "paths.hpp"
#include <algorithm>
#include <iterator>
#include <vector>
#include "autocomplete.hpp"

AutoComplete::AutoComplete() {
    this->initExecutableList();

    for (const auto &cmd : executableList) {
        trie.insertWord(cmd);
    }
}

std::vector<std::string> AutoComplete::match(const std::string &usrInput) const {
    return trie.getSuggestions(usrInput);
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

void AutoComplete::initExecutableList() {
    executableList = {"echo", "type", "pwd", "cd", "exit"};
    Paths paths;

    const std::vector<std::string> &execPaths = paths.getExecutablesInPathEnv();

    std::ranges::move(execPaths, std::back_inserter(executableList));
}
