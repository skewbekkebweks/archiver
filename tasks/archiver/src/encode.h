#pragma once

#include "trie.h"
#include "write_bit_stream.h"

#include <cstdint>
#include <string>
#include <vector>
#include <map>

bool CompareCodes(const std::pair<uint16_t, int>& lhs, const std::pair<uint16_t, int>& rhs);

TrieNode* BuildTrie(const std::map<uint16_t, int>& frequency_map);

std::map<uint16_t, uint16_t> GetCodesSizeCount(const std::map<uint16_t, int>& frequency_map);

std::vector<std::pair<uint16_t, std::string>> GenerateCodes(const std::map<uint16_t, int>& frequency_map);

std::map<uint16_t, int> GenerateFrequencyMap(const std::string& filename, int coef_speacial = 1, bool add_name = true);

void WriteFile(WriteBitStream& writer, const std::string filename,
               const std::vector<std::pair<uint16_t, std::string>>& ordered_codes,
               const std::map<uint16_t, std::string>& codes, const std::map<size_t, int16_t>& count_with_size,
               size_t max_sybmol_code_size);

void Encode(const std::string& archive_name, const std::vector<std::string>& filenames);