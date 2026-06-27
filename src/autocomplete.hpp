#pragma once

#include "trie.hpp"
#include "paths.hpp"
#include <string>
#include <vector>

class AutoComplete {
  public:
    AutoComplete();

    [[nodiscard]] std::vector<std::string> execMatch(const std::string &usrInput) const;
    [[nodiscard]] std::string lcp(const std::vector<std::string> &words) const;

    [[nodiscard]] std::string getAbsolutePath() const;

  private:
    std::vector<std::string> executableList;
    BasicTrie trieForExecutables;

    void initExecutableList();

    Paths paths;
};
