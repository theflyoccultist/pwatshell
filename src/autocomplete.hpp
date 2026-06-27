#pragma once

#include "fileinfo.hpp"
#include "trie.hpp"
#include "paths.hpp"
#include <string>
#include <vector>

class AutoComplete {
  public:
    AutoComplete();

    [[nodiscard]] std::vector<FileInfo> execMatch(const std::string &usrInput) const;
    [[nodiscard]] std::string lcp(const std::vector<FileInfo> &words) const;

    [[nodiscard]] std::string getAbsolutePath() const;

  private:
    std::vector<FileInfo> executableList;
    BasicTrie trieForExecutables;

    void initExecutableList();

    Paths paths;
};
