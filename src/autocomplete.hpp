#pragma once

#include "fileinfo.hpp"
#include "trie.hpp"
#include "paths.hpp"
#include <string>
#include <vector>

class AutoComplete {
  public:
    AutoComplete();

    [[nodiscard]] std::vector<FileInfo> match(const std::string &usrInput) const;
    [[nodiscard]] std::string lcp(const std::vector<FileInfo> &words) const;
    [[nodiscard]] std::vector<FileInfo> matchFilesInDirectory(const std::string &usrInput);

  private:
    std::vector<FileInfo> executableList;
    BasicTrie trieForExecutables;
    BasicTrie trieForFiles;

    void initExecutableList();

    Paths paths;
};
