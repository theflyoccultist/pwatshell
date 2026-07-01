#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
#include "autocomplete.hpp"
#include "opts.hpp"

AutoComplete::AutoComplete() {
    this->initExecutableList();

    for (const auto &exe : executableList) {
        trieForExecutables.insert(exe);
    }
}

std::vector<std::string> AutoComplete::execMatch(const std::string &usrInput) const {
    return trieForExecutables.getSuggestions(usrInput);
}

void AutoComplete::initExecutableList() {
    // add shell built in names to list
    for (const auto &builtin : opts::optionsMap) {
        executableList.emplace_back(builtin.first);
    }

    // add PATH executable names to list
    const std::vector<std::string> &execNames = paths.getExecutablesInPathEnv();
    std::ranges::move(execNames, std::back_inserter(executableList));
}
