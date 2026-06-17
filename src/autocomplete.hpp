#include "trie.hpp"
#include <string>
#include <vector>

class AutoComplete {
  public:
    AutoComplete() {
        for (auto cmd : {"echo", "type", "pwd", "cd", "exit"}) {
            trie.insertWord(cmd);
        }
    }

    [[nodiscard]] std::vector<std::string> match(const std::string &usrInput) const {
        return trie.getSuggestions(usrInput);
    }

  private:
    BasicTrie<char> trie;
};
