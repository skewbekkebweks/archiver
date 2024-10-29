#include "../../src/trie.cpp"

#include <iostream>
#include <memory>

int main() {
    const std::string red = "\033[31m";
    const std::string green = "\033[32m";
    const std::string reset = "\033[0m";
    const std::string passed = "PASSED";
    const std::string failed = "FAILED";

    std::cout << "Trie test" << std::endl;

    {
        std::cout << "Test 1: A single leaf node" << std::endl;
        TrieNode* root = new TrieNode{0};
        std::map<uint16_t, uint16_t> codes_size_count;
        FillCodesSizeCount(codes_size_count, 1, root);
        
        if (codes_size_count[1] == 1) {
            std::cout << green << passed << reset << std::endl;
        } else {
            std::cout << red << failed << " codes_size_count[1] should be 1 but it is not" << reset << std::endl;
        }
    }

    {
        std::cout << "Test 2: A tree with more nodes, depth 1" << std::endl;
        TrieNode* root = new TrieNode{NONE_SYMBOL};
        root->left = new TrieNode{0};
        root->right = new TrieNode{1};
        
        std::map<uint16_t, uint16_t> codes_size_count;
        FillCodesSizeCount(codes_size_count, 1, root);

        if (codes_size_count[1] == 0) {
            std::cout << green << passed << reset << std::endl;
        } else {
            std::cout << red << failed << " codes_size_count[1] should be 0 but it is not" << reset << std::endl;
        }
        if (codes_size_count[2] == 2) {
            std::cout << green << passed << reset << std::endl;
        } else {
            std::cout << red << failed << " codes_size_count[2] should be 2 but it is not" << reset << std::endl;
        }
    }

    {
        std::cout << "Test 3: A tree with more levels" << std::endl;
        TrieNode* root = new TrieNode{NONE_SYMBOL};
        root->left = new TrieNode{NONE_SYMBOL};
        root->right = new TrieNode{0};
        root->left->left = new TrieNode{1};
        root->left->right = new TrieNode{2};

        std::map<uint16_t, uint16_t> codes_size_count;
        FillCodesSizeCount(codes_size_count, 1, root);
        
        if (codes_size_count[1] == 0) {
            std::cout << green << passed << reset << std::endl;
        } else {
            std::cout << red << failed << " codes_size_count[1] should be 0 but it is not" << reset << std::endl;
        }
        if (codes_size_count[2] == 1) {
            std::cout << green << passed << reset << std::endl;
        } else {
            std::cout << red << failed << " codes_size_count[2] should be 1 but it is not" << reset << std::endl;
        }
        if (codes_size_count[3] == 2) {
            std::cout << green << passed << reset << std::endl;
        } else {
            std::cout << red << failed << " codes_size_count[3] should be 2 but it is not" << reset << std::endl;
        }
    }

    {
        std::cout << "Test 4: A complete binary tree" << std::endl;
        TrieNode* root = new TrieNode{NONE_SYMBOL};
        root->left = new TrieNode{NONE_SYMBOL};
        root->right = new TrieNode{NONE_SYMBOL};
        root->left->left = new TrieNode{1};
        root->left->right = new TrieNode{2};
        root->right->left = new TrieNode{3};
        root->right->right = new TrieNode{4};

        std::map<uint16_t, uint16_t> codes_size_count;
        FillCodesSizeCount(codes_size_count, 1, root);
        
        if (codes_size_count[1] == 0) {
            std::cout << green << passed << reset << std::endl;
        } else {
            std::cout << red << failed << " codes_size_count[1] should be 0 but it is not" << reset << std::endl;
        }
        if (codes_size_count[2] == 0) {
            std::cout << green << passed << reset << std::endl;
        } else {
            std::cout << red << failed << " codes_size_count[2] should be 0 but it is not" << reset << std::endl;
        }
        if (codes_size_count[3] == 4) {
            std::cout << green << passed << reset << std::endl;
        } else {
            std::cout << red << failed << " codes_size_count[3] should be 4 but it is not" << reset << std::endl;
        }
    }
}