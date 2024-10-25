#pragma once

#include "trie.h"

#include <cstdint>
#include <map>

bool CompareCodes(const std::pair<uint16_t, int>& lhs, const std::pair<uint16_t, int>& rhs);

TrieNode* BuildTrie(const std::map<uint16_t, int>& frequency_map);

std::map<uint16_t, uint16_t> GetCodesSizeCount(const std::map<uint16_t, int>& frequency_map);

std::vector<std::pair<uint16_t, std::string>> GenerateCodes(const std::map<uint16_t, int>& frequency_map);

std::map<uint16_t, int> GenerateFrequencyMap(const std::string& filename, int coef_speacial = 1, bool add_name = true);

void Encode(const std::string& archive_name, const std::vector<std::string>& filenames);