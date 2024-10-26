#pragma once

#include <limits>
#include <map>
#include <cstdint>
#include <memory>

const uint32_t NONE_SYMBOL = std::numeric_limits<uint32_t>::max();

struct TrieNode {
    uint32_t symbol;  // NONE_SYMBOL if None

    std::shared_ptr<TrieNode> left;
    std::shared_ptr<TrieNode> right;

    explicit TrieNode(uint32_t symbol, std::shared_ptr<TrieNode> left = nullptr, 
             std::shared_ptr<TrieNode> right = nullptr)
        : symbol(symbol), left(left), right(right) {}
};


void FillCodesSizeCount(std::map<uint16_t, uint16_t>& codes_size_count, int cur_depth, std::shared_ptr<TrieNode> cur_node);

// void Clear(TrieNode* cur_node);