#include "paths.hpp"
#include <vector>
#include "autocomplete.hpp"

AutoComplete::AutoComplete() {
    initExecutableList();

    for (const auto &cmd : executableList) {
        trie.insertWord(cmd);
    }
}

void AutoComplete::initExecutableList() {
    executableList = {"echo", "type", "pwd", "cd", "exit"};
    Paths paths;

    const std::vector<std::string> &execPaths = paths.getExecutablesInPathEnv();

    for (auto &path : execPaths) {
        executableList.push_back(path);
    }
}
