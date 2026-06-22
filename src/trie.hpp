#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "fileinfo.hpp"

class BasicTrie {
  private:
    struct TrieNode {
        std::unordered_map<char, std::unique_ptr<TrieNode>> children;
        std::unique_ptr<FileInfo> fileData = nullptr;
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

    void collectFilesHelper(const TrieNode *node, std::vector<FileInfo> &results) const {
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

    void insert(const FileInfo &info) const {
        TrieNode *current = root.get();

        for (char ch : info.filename) {
            if (current->children.find(ch) == current->children.end()) {
                current->children[ch] = std::make_unique<TrieNode>();
            }

            current = current->children[ch].get();
        }
        current->fileData = std::make_unique<FileInfo>(info);
    }

    [[nodiscard]] std::vector<FileInfo> getSuggestions(const std::string &prefix) const {
        std::vector<FileInfo> results;

        // Find where the user's current input ends in the tree
        const TrieNode *prefixNode = findPrefixNode(prefix);

        // If it doesn't exist, return an empty vector
        if (!prefixNode)
            return results;

        // Gather all words stemming from this node
        collectFilesHelper(prefixNode, results);

        std::ranges::sort(
            results, [](const FileInfo &a, const FileInfo &b) { return a.filename < b.filename; });

        return results;
    }

    void clar() { root = std::make_unique<TrieNode>(); }
};
