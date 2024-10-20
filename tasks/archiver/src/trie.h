#include <map>
#include <string>

struct TrieNode {
    int frequency;
    TrieNode* left;
    TrieNode* right;
};

struct TrieNodeCompare {
    bool operator()(const TrieNode* lhs, const TrieNode* rhs);
};

void FillCodesSizeCount(std::map<uint16_t, uint16_t>& codes_size_count, int cur_depth, TrieNode* cur_node);

void Clear(TrieNode* cur_node);