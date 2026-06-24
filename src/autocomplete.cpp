#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
#include "autocomplete.hpp"
#include "fileinfo.hpp"

AutoComplete::AutoComplete() {
    this->initExecutableList();

    for (const auto &exe : executableList) {
        trieForExecutables.insert(exe);
    }
}

std::vector<FileInfo> AutoComplete::execMatch(const std::string &usrInput) const {
    return trieForExecutables.getSuggestions(usrInput);
}

std::string AutoComplete::lcp(const std::vector<FileInfo> &words) const {
    if (words.empty()) {
        return "";
    }

    if (words.size() == 1) {
        return words[0].filename;
    }

    for (size_t charIndex = 0; charIndex < words[0].filename.size(); ++charIndex) {
        for (size_t stringIndex = 1; stringIndex < words.size(); ++stringIndex) {
            if (words[stringIndex].filename.size() <= charIndex ||
                words[stringIndex].filename[charIndex] != words[0].filename[charIndex]) {
                return words[0].filename.substr(0, charIndex);
            }
        }
    }

    return words[0].filename;
}

std::vector<FileInfo> AutoComplete::fileMatch(const std::string &usrInput) {
    trieForFiles.clear();
    const std::vector<FileInfo> &files = paths.getFilesInCurrPath();

    for (const auto &file : files) {
        trieForFiles.insert(file);
    }

    return trieForFiles.getSuggestions(usrInput);
}

void AutoComplete::initExecutableList() {
    // built ins
    executableList.emplace_back("echo", false);
    executableList.emplace_back("type", false);
    executableList.emplace_back("pwd", false);
    executableList.emplace_back("cd", false);
    executableList.emplace_back("exit", false);

    const std::vector<FileInfo> &execNames = paths.getExecutablesInPathEnv();

    std::ranges::move(execNames, std::back_inserter(executableList));
}
