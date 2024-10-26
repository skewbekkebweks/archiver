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
        std::shared_ptr<TrieNode> root = std::make_shared<TrieNode>(0, nullptr, nullptr);
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
        std::shared_ptr<TrieNode> root = std::make_shared<TrieNode>(NONE_SYMBOL, nullptr, nullptr);
        root->left = std::make_shared<TrieNode>(0, nullptr, nullptr);
        root->right = std::make_shared<TrieNode>(1, nullptr, nullptr);
        
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
        std::shared_ptr<TrieNode> root = std::make_shared<TrieNode>(NONE_SYMBOL, nullptr, nullptr);
        root->left = std::make_shared<TrieNode>(NONE_SYMBOL, nullptr, nullptr);
        root->right = std::make_shared<TrieNode>(0, nullptr, nullptr);
        root->left->left = std::make_shared<TrieNode>(1, nullptr, nullptr);
        root->left->right = std::make_shared<TrieNode>(2, nullptr, nullptr);

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
        std::shared_ptr<TrieNode> root = std::make_shared<TrieNode>(NONE_SYMBOL, nullptr, nullptr);
        root->left = std::make_shared<TrieNode>(NONE_SYMBOL, nullptr, nullptr);
        root->right = std::make_shared<TrieNode>(NONE_SYMBOL, nullptr, nullptr);
        root->left->left = std::make_shared<TrieNode>(1, nullptr, nullptr);
        root->left->right = std::make_shared<TrieNode>(2, nullptr, nullptr);
        root->right->left = std::make_shared<TrieNode>(3, nullptr, nullptr);
        root->right->right = std::make_shared<TrieNode>(4, nullptr, nullptr);

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