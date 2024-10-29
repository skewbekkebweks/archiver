#pragma once

#include <limits>
#include <map>
#include <cstdint>
#include <memory>

const uint32_t NONE_SYMBOL = std::numeric_limits<uint32_t>::max();

struct TrieNode {
    uint32_t symbol;  // NONE_SYMBOL if None

    TrieNode* left;
    TrieNode* right;

    explicit TrieNode(uint32_t symbol, TrieNode* left = nullptr, TrieNode* right = nullptr)
        : symbol(symbol), left(left), right(right) {
    }
};

struct Trie {
    TrieNode* root;

    ~Trie();

    void Clear(TrieNode* node);
    void Clear();
};

void FillCodesSizeCount(std::map<uint16_t, uint16_t>& codes_size_count, int cur_depth, TrieNode* cur_node);
