#include "paths.hpp"
#include <algorithm>
#include <iterator>
#include <vector>
#include "autocomplete.hpp"

AutoComplete::AutoComplete() {
    initExecutableList();

    for (const auto &cmd : executableList) {
        trie.insertWord(cmd);
    }
}

std::vector<std::string> AutoComplete::match(const std::string &usrInput) const {
    return trie.getSuggestions(usrInput);
}

void AutoComplete::initExecutableList() {
    executableList = {"echo", "type", "pwd", "cd", "exit"};
    Paths paths;

    const std::vector<std::string> &execPaths = paths.getExecutablesInPathEnv();

    std::ranges::move(execPaths, std::back_inserter(executableList));
}
