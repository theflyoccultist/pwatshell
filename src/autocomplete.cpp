#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
#include "autocomplete.hpp"

AutoComplete::AutoComplete() {
    this->initExecutableList();

    for (const auto &exe : executableList) {
        trieForExecutables.insert(exe);
    }
}

std::vector<std::string> AutoComplete::execMatch(const std::string &usrInput) const {
    return trieForExecutables.getSuggestions(usrInput);
}

std::string AutoComplete::getAbsolutePath() const { return paths.pwd(); }

void AutoComplete::initExecutableList() {
    // built ins
    executableList.emplace_back("echo");
    executableList.emplace_back("type");
    executableList.emplace_back("pwd");
    executableList.emplace_back("cd");
    executableList.emplace_back("exit");

    const std::vector<std::string> &execNames = paths.getExecutablesInPathEnv();

    std::ranges::move(execNames, std::back_inserter(executableList));
}
