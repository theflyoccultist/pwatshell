#pragma once

#include "trie.hpp"
#include "paths.hpp"
#include <string>
#include <vector>

class AutoComplete {
  public:
    AutoComplete();

    [[nodiscard]] std::vector<std::string> match(const std::string &usrInput) const;
    [[nodiscard]] std::string lcp(const std::vector<std::string> &words) const;
    [[nodiscard]] std::vector<std::string> matchFilesInDirectory(const std::string &usrInput) const;

  private:
    BasicTrie<char> trieForExecutables;
    BasicTrie<char> trieForFiles;
    std::vector<std::string> executableList;

    void initExecutableList();

    Paths paths;
};
