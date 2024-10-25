#pragma once

#include "read_bit_stream.h"
#include "trie.h"

#include <cstdint>

uint16_t ForceGetFromReader(ReadBitStream& bs, int bits_cnt);

uint16_t GetSymbolFromCodes(TrieNode* root, ReadBitStream& bs);

TrieNode* BuildTrie(const std::map<std::string, uint16_t>& codes);

void Decode(const std::string& archive_name);