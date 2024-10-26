#include "encode.h"
#include "utility.h"
#include "read_bit_stream.h"
#include "write_bit_stream.h"

#include <memory>
#include <queue>
#include <utility>
#include "heap.h"

#include <algorithm>
#include <filesystem>

bool CompareCodes(const std::pair<uint16_t, int>& lhs, const std::pair<uint16_t, int>& rhs) {
    return lhs.second < rhs.second;
}

std::shared_ptr<TrieNode> BuildTrie(const std::map<uint16_t, int>& frequency_map) {
    Heap<std::pair<uint64_t, std::shared_ptr<TrieNode>>, std::less<std::pair<uint64_t, std::shared_ptr<TrieNode>>>>
        frequencies;
    for (auto [symbol, frequency] : frequency_map) {
        frequencies.Push(std::make_pair(frequency, std::make_shared<TrieNode>(symbol, nullptr, nullptr)));
    }

    while (frequencies.Size() > 1) {
        uint64_t left_frequency = frequencies.Top().first;
        std::shared_ptr<TrieNode> left = frequencies.Top().second;
        frequencies.Pop();
        uint64_t right_frequency = frequencies.Top().first;
        std::shared_ptr<TrieNode> right = frequencies.Top().second;
        frequencies.Pop();

        std::shared_ptr<TrieNode> parent = std::make_shared<TrieNode>(NONE_SYMBOL, left, right);

        frequencies.Push(std::make_pair(left_frequency + right_frequency, parent));
    }

    std::shared_ptr<TrieNode> root = frequencies.Top().second;
    return root;
}

std::map<uint16_t, uint16_t> GetCodesSizeCount(const std::map<uint16_t, int>& frequency_map) {
    std::shared_ptr<TrieNode> root = BuildTrie(frequency_map);
    std::map<uint16_t, uint16_t> codes_size_count;
    FillCodesSizeCount(codes_size_count, 0, root);
    // Clear(root);

    return codes_size_count;
}

std::vector<std::pair<uint16_t, std::string>> GenerateCodes(const std::map<uint16_t, int>& frequency_map) {
    std::vector<std::pair<uint16_t, int>> sorted_symbols;
    for (auto [symbol, frequency] : frequency_map) {
        sorted_symbols.emplace_back(symbol, frequency);
    }
    std::sort(sorted_symbols.begin(), sorted_symbols.end(), CompareCodes);
    std::reverse(sorted_symbols.begin(), sorted_symbols.end());

    std::map<uint16_t, uint16_t> codes_size_count = GetCodesSizeCount(frequency_map);

    uint16_t symbols_count = sorted_symbols.size();
    uint16_t symbols_generated_count = 0;

    std::string current_code;

    std::vector<std::pair<uint16_t, std::string>> codes;

    uint16_t current_symbol_size = 1;
    while (symbols_generated_count != symbols_count) {
        current_code += '0';
        uint16_t count_symbols_current_size = codes_size_count[current_symbol_size];

        for (uint16_t i = 0; i < count_symbols_current_size; ++i) {
            codes.emplace_back(sorted_symbols[symbols_generated_count].first, current_code);
            symbols_generated_count++;

            IncrementBinaryString(current_code);
        }

        current_symbol_size++;
    }
    return codes;
}

std::map<uint16_t, int> GenerateFrequencyMap(const std::string& filename, int coef_speacial, bool add_name) {
    std::map<uint16_t, int> frequency_map;

    frequency_map[FILENAME_END] = coef_speacial;
    frequency_map[ONE_MORE_FILE] = coef_speacial;
    frequency_map[ARCHIVE_END] = coef_speacial;

    std::ifstream file(filename);
    if (!file.good()) {
        throw FileBroken{filename};
    }
    ReadBitStream file_reader{file};

    uint16_t c = 0;
    while (file_reader.ReadBits(std::numeric_limits<unsigned char>::digits, c)) {
        frequency_map[c]++;
    }

    if (add_name) {
        std::string filename_wo_path = std::filesystem::path(filename).filename().string();

        for (char c : filename_wo_path) {
            frequency_map[c]++;
        }
    }

    return frequency_map;
}

void Encode(const std::string& archive_name, const std::vector<std::string>& filenames) {
    std::ofstream archive_out(archive_name);
    if (!archive_out.good()) {
        throw FileBroken{archive_name};
    }
    WriteBitStream archive_writer{archive_out};

    for (size_t i = 0; i < filenames.size(); ++i) {
        const std::string filename = filenames[i];

        std::map<uint16_t, int> frequency_map = GenerateFrequencyMap(filename);

        std::vector<std::pair<uint16_t, std::string>> ordered_codes = GenerateCodes(frequency_map);

        std::map<uint16_t, std::string> codes;
        std::map<size_t, int16_t> count_with_size;

        size_t max_sybmol_code_size = 0;
        for (const auto& [symbol, code] : ordered_codes) {

            codes[symbol] = code;
            count_with_size[code.size()]++;
            max_sybmol_code_size = std::max(max_sybmol_code_size, code.size());
        }

        archive_writer.WriteBits(BITS_IN_ITEM, ordered_codes.size());

        for (auto [symbol, code] : ordered_codes) {
            archive_writer.WriteBits(BITS_IN_ITEM, symbol);
        }

        for (size_t size = 1; size <= max_sybmol_code_size; ++size) {
            archive_writer.WriteBits(BITS_IN_ITEM, count_with_size[size]);
        }

        std::string filename_wo_path = std::filesystem::path(filename).filename().string();
        for (char c : filename_wo_path) {
            archive_writer.WriteSeq(codes[c]);
        }
        archive_writer.WriteSeq(codes[FILENAME_END]);

        std::ifstream file(filename);
        if (!file.good()) {
            throw FileBroken{filename};
        }
        ReadBitStream file_reader{file};

        uint16_t c = 0;
        while (file_reader.ReadBits(std::numeric_limits<unsigned char>::digits, c)) {
            archive_writer.WriteSeq(codes[c]);
        }

        if (i == filenames.size() - 1) {
            archive_writer.WriteSeq(codes[ARCHIVE_END]);
        } else {
            archive_writer.WriteSeq(codes[ONE_MORE_FILE]);
        }
    }
}