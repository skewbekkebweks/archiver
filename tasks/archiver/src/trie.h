#pragma once

#include <_types/_uint32_t.h>
#include <limits>
#include <map>
#include <cstdint>

const uint32_t NONE_SYMBOL = std::numeric_limits<uint32_t>::max();

struct TrieNode {
    uint32_t symbol;  // NONE_SYMBOL if None
    TrieNode* left;
    TrieNode* right;
};


void FillCodesSizeCount(std::map<uint16_t, uint16_t>& codes_size_count, int cur_depth, TrieNode* cur_node);

void Clear(TrieNode* cur_node);