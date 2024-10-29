#include "trie.h"
#include <memory>

void Trie::Clear(TrieNode* node) {
    if (node == nullptr) {
        return;
    }

    Clear(node->left);
    Clear(node->right);

    delete node;
}

void Trie::Clear() {
    Clear(root);
}

Trie::~Trie() {
    Clear();
}

void FillCodesSizeCount(std::map<uint16_t, uint16_t>& codes_size_count, int cur_depth,
                        TrieNode* cur_node) {
    if (cur_node->left == nullptr) {
        codes_size_count[cur_depth]++;
        return;
    }

    FillCodesSizeCount(codes_size_count, cur_depth + 1, cur_node->left);
    FillCodesSizeCount(codes_size_count, cur_depth + 1, cur_node->right);
}
