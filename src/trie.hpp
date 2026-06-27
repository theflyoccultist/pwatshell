#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

class BasicTrie {
  private:
    struct TrieNode {
        std::unordered_map<char, std::unique_ptr<TrieNode>> children;
        std::unique_ptr<std::string> fileData = nullptr;
    };

    std::unique_ptr<TrieNode> root;

    [[nodiscard]] const TrieNode *findPrefixNode(const std::string &prefix) const {
        const TrieNode *current = root.get();

        for (char ch : prefix) {
            auto it = current->children.find(ch);
            if (it == current->children.end()) {
                return nullptr;
            }

            current = it->second.get();
        }

        return current;
    }

    void collectFilesHelper(const TrieNode *node, std::vector<std::string> &results) const {
        if (!node)
            return;

        if (node->fileData) {
            results.push_back(*(node->fileData));
        }

        for (const auto &entry : node->children) {
            collectFilesHelper(entry.second.get(), results);
        }
    }

  public:
    BasicTrie() : root(std::make_unique<TrieNode>()) {}

    void insert(const std::string &info) const {
        TrieNode *current = root.get();

        for (char ch : info) {
            if (current->children.find(ch) == current->children.end()) {
                current->children[ch] = std::make_unique<TrieNode>();
            }

            current = current->children[ch].get();
        }
        current->fileData = std::make_unique<std::string>(info);
    }

    [[nodiscard]] std::vector<std::string> getSuggestions(const std::string &prefix) const {
        std::vector<std::string> results;

        // Find where the user's current input ends in the tree
        const TrieNode *prefixNode = findPrefixNode(prefix);

        // If it doesn't exist, return an empty vector
        if (!prefixNode)
            return results;

        // Gather all words stemming from this node
        collectFilesHelper(prefixNode, results);

        std::ranges::sort(results);

        return results;
    }

    void clear() { root = std::make_unique<TrieNode>(); }
};
