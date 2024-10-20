#include <_types/_uint16_t.h>
#include <map>
#include <string>

const uint16_t NULL_SYMBOL = UINT16_MAX;

struct TrieNode {
    int frequency;
    TrieNode* left;
    TrieNode* right;
};

struct TrieNodeCompare {
    bool operator()(const TrieNode* lhs, const TrieNode* rhs) {
        return lhs->frequency > rhs->frequency;
    }
};


void FillCodes(std::map<uint16_t, uint16_t>& codes, int cur_depth, TrieNode* cur_node) {
    if (cur_node->left == nullptr) {
        codes[cur_depth]++;
        return;
    }

    FillCodes(codes, cur_depth + 1, cur_node->left);
    FillCodes(codes, cur_depth + 1, cur_node->right);

    delete cur_node;
} 