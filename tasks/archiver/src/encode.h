#pragma once

#include "trie.h"

#include <cstdint>
#include <map>

bool CompareCodes(const std::pair<uint16_t, int>& lhs, const std::pair<uint16_t, int>& rhs);

TrieNode* BuildTrie(std::map<uint16_t, int> frequency_map);

std::map<uint16_t, uint16_t> GetCodesSizeCount(std::map<uint16_t, int> frequency_map);

std::vector<std::pair<uint16_t, std::string>> GenerateCodes(std::map<uint16_t, int> frequency_map);

std::map<uint16_t, int> GenerateFrequencyMap(std::string filename, int coef_speacial = 1, bool add_name = true);

void Encode(std::string archive_name, std::vector<std::string> filenames);