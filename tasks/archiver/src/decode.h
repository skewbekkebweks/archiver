#pragma once

#include "read_bit_stream.h"
#include "trie.h"

#include <cstdint>
#include <string>
#include <vector>
#include <map>

uint16_t ForceGetFromReader(ReadBitStream& bs, int bits_cnt);

uint16_t GetSymbolFromCodes(TrieNode* root, ReadBitStream& bs);

std::shared_ptr<TrieNode> BuildTrie(const std::map<std::string, uint16_t>& codes);

void Decode(const std::string& archive_name);