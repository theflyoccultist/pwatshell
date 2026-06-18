#pragma once

#include "trie.hpp"
#include <string>
#include <vector>

class AutoComplete {
  public:
    AutoComplete();

    [[nodiscard]] std::vector<std::string> match(const std::string &usrInput) const;
    [[nodiscard]] std::string lcp(const std::vector<std::string> &words) const;

  private:
    BasicTrie<char> trie;
    std::vector<std::string> executableList;
    void initExecutableList();
};
