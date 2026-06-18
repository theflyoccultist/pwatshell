#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

template <typename CharType = char> class BasicTrie {
  private:
    struct TrieNode {
        std::unordered_map<CharType, std::unique_ptr<TrieNode>> children;
        bool isEndOfWord = false;
    };

    std::unique_ptr<TrieNode> root;

    const TrieNode *findPrefixNode(const std::basic_string<CharType> &prefix) const {
        const TrieNode *current = root.get();

        for (CharType ch : prefix) {
            auto it = current->children.find(ch);
            if (it == current->children.end()) {
                return nullptr;
            }

            current = it->second.get();
        }

        return current;
    }

    void collectWordsHelper(const TrieNode *node, std::basic_string<CharType> currentWord,
                            std::vector<std::basic_string<CharType>> &results) const {
        if (!node)
            return;

        if (node->isEndOfWord) {
            results.push_back(currentWord);
        }

        for (const auto &entry : node->children) {
            collectWordsHelper(entry.second.get(), currentWord + entry.first, results);
        }
    }

  public:
    BasicTrie() : root(std::make_unique<TrieNode>()) {}

    void insertWord(const std::basic_string<CharType> &word) const {
        TrieNode *current = root.get();

        for (CharType ch : word) {
            if (current->children.find(ch) == current->children.end()) {
                current->children[ch] = std::make_unique<TrieNode>();
            }

            current = current->children[ch].get();
        }
        current->isEndOfWord = true;
    }

    std::vector<std::basic_string<CharType>>
    getSuggestions(const std::basic_string<CharType> &prefix) const {
        std::vector<std::basic_string<CharType>> results;

        // Find where the user's current input ends in the tree
        const TrieNode *prefixNode = findPrefixNode(prefix);

        // If it doesn't exist, return an empty vector
        if (!prefixNode)
            return results;

        // Gather all words stemming from this node
        collectWordsHelper(prefixNode, prefix, results);

        std::ranges::sort(results);
        return results;
    }
};
