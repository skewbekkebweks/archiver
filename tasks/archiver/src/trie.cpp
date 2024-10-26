#include "trie.h"
#include <memory>

void FillCodesSizeCount(std::map<uint16_t, uint16_t>& codes_size_count, int cur_depth, std::shared_ptr<TrieNode> cur_node) {
    if (cur_node->left == nullptr) {
        codes_size_count[cur_depth]++;
        return;
    }

    FillCodesSizeCount(codes_size_count, cur_depth + 1, cur_node->left);
    FillCodesSizeCount(codes_size_count, cur_depth + 1, cur_node->right);
}

// void Clear(TrieNode* cur_node) {
//     if (cur_node != nullptr) {
//         Clear(cur_node->left);
//         Clear(cur_node->right);
//         delete cur_node;
//     }
// }