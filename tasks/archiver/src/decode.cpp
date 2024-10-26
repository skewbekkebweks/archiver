#include "decode.h"
#include <memory>

#include "exception.h"
#include "read_bit_stream.h"
#include "utility.h"
#include "write_bit_stream.h"

uint16_t ForceGetFromReader(ReadBitStream& bs, int bits_cnt) {
    uint16_t number = 0;
    if (bs.ReadBits(bits_cnt, number)) {
        return number;
    }

    throw IncorrectArchiveDataFormat{};
}

uint16_t GetSymbolFromCodes(std::shared_ptr<TrieNode> root, ReadBitStream& bs) {
    std::shared_ptr<TrieNode> current = root;

    while (current->symbol == NONE_SYMBOL) {
        bool turn = ForceGetFromReader(bs, 1);
        if (turn) {
            if (current->right == nullptr) {
                throw IncorrectArchiveDataFormat{};
            }
            current = current->right;
        } else {
            if (current->left == nullptr) {
                throw IncorrectArchiveDataFormat{};
            }
            current = current->left;
        }
    }

    return current->symbol;
}

std::shared_ptr<TrieNode> BuildTrie(const std::map<std::string, uint16_t>& codes) {
    std::shared_ptr<TrieNode> root = std::make_shared<TrieNode>(NONE_SYMBOL, nullptr, nullptr);

    for (const auto& [code, symbol] : codes) {
        std::shared_ptr<TrieNode> cur_node = root;
        for (char c : code) {
            if (cur_node->left == nullptr) {
                cur_node->left = std::make_shared<TrieNode>(NONE_SYMBOL, nullptr, nullptr);
                cur_node->right = std::make_shared<TrieNode>(NONE_SYMBOL, nullptr, nullptr);
            }
            if (c == '0') {
                cur_node = cur_node->left;
            } else {
                cur_node = cur_node->right;
            }
        }
        cur_node->symbol = symbol;
    }

    return root;
}

std::map<std::string, uint16_t> GetCodesFromReader(ReadBitStream& reader) {
    std::map<std::string, uint16_t> codes;

    uint16_t symbols_count = ForceGetFromReader(reader, BITS_IN_ITEM);

    std::vector<uint16_t> symbols(symbols_count);
    for (int i = 0; i < symbols_count; ++i) {
        symbols[i] = ForceGetFromReader(reader, BITS_IN_ITEM);
    }

    uint16_t symbols_read_count = 0;

    std::string current_code;

    while (symbols_read_count != symbols_count) {
        current_code += '0';

        uint16_t count_symbols_current_size = ForceGetFromReader(reader, BITS_IN_ITEM);

        for (uint16_t i = 0; i < count_symbols_current_size; ++i) {
            codes[current_code] = symbols[symbols_read_count];
            symbols_read_count++;

            IncrementBinaryString(current_code);
        }
    }

    return codes;
}

void Decode(const std::string& archive_name) {
    std::ifstream archive_in(archive_name);
    if (!archive_in.good()) {
        throw FileBroken{archive_name};
    }
    ReadBitStream archive_reader{archive_in};

    while (true) {
        std::map<std::string, uint16_t> codes = GetCodesFromReader(archive_reader);

        std::shared_ptr<TrieNode> huffman_tree_root = BuildTrie(codes);

        try {
            uint16_t next_symbol = 0;

            std::string filename;
            while ((next_symbol = GetSymbolFromCodes(huffman_tree_root, archive_reader)) != FILENAME_END) {
                filename += static_cast<char>(next_symbol);
            }

            std::ofstream file_out(filename);

            WriteBitStream file_writer(file_out);

            while (true) {
                next_symbol = GetSymbolFromCodes(huffman_tree_root, archive_reader);
                if (next_symbol == ONE_MORE_FILE) {
                    break;
                }
                if (next_symbol == ARCHIVE_END) {
                    return;
                }

                file_writer.WriteBits(std::numeric_limits<unsigned char>::digits, static_cast<char>(next_symbol));
            }
        } catch (std::exception& ex) {
            // Clear(huffman_tree_root);
            throw ex;
        }

        // Clear(huffman_tree_root);
    }
}