#include "args_parser.h"
#include "exception.h"
#include "bit_stream.h"
#include "trie.h"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <optional>
#include <ostream>
#include <string>
#include <map>
#include <filesystem>
#include <queue>
#include <vector>
#include <algorithm>

const int SUCCESS_EXIT_CODE = 0;
const int FAILED_EXIT_CODE = 111;

const std::string ENCODE_FLAG = "-c";
const std::string DECODE_FLAG = "-d";
const std::string HELP_FLAG = "-h";

const int FILENAME_END = 256;
const int ONE_MORE_FILE = 257;
const int ARCHIVE_END = 258;

const int BITS_IN_ITEM = 9;

bool CompareCodes(const std::pair<uint16_t, int>& lhs, const std::pair<uint16_t, int>& rhs) {
    return lhs.second < rhs.second;
}

uint16_t ForceGetFromReader(BitStream& bs, int bits_cnt) {
    std::optional<uint16_t> number = bs.ReadBits(bits_cnt);
    if (number.has_value()) {
        return number.value();
    }
    
    throw IncorrectFileDataFormat{};
}

uint16_t GetSymbolFromCodes(std::map<std::string, uint16_t> codes, BitStream& bs) {
    std::string s;

    while (!codes.contains(s)) {
        s += (ForceGetFromReader(bs, 1) ? '1' : '0');
    }

    return codes[s];
}

void UpdateCodes(std::vector<std::string>& current_free_codes, uint16_t starts_with = 0) {
    std::vector<std::string> next_free_codes;
    for (size_t i = starts_with; i < current_free_codes.size(); ++i) {
        next_free_codes.push_back(current_free_codes[i] + "0");
        next_free_codes.push_back(current_free_codes[i] + "1");
    }

    swap(current_free_codes, next_free_codes);
}

TrieNode* BuildTrie(std::map<uint16_t, int> frequency_map) {
    std::priority_queue<TrieNode*, std::vector<TrieNode*>, TrieNodeCompare> frequencies;
    for (auto [symbol, frequency] : frequency_map) {
        frequencies.emplace(new TrieNode{frequency, nullptr, nullptr});
    }

    while (frequencies.size() > 1) {
        TrieNode* left = frequencies.top();
        frequencies.pop();
        TrieNode* right = frequencies.top();
        frequencies.pop();

        TrieNode* parent = new TrieNode{left->frequency + right->frequency, left, right};

        frequencies.emplace(parent);
    }

    TrieNode* root = frequencies.top();
    return root;
}

std::map<uint16_t, uint16_t> GetCodesSizeCount(std::map<uint16_t, int> frequency_map) {
    TrieNode* root = BuildTrie(frequency_map);
    std::map<uint16_t, uint16_t> codes_size_count;
    FillCodesSizeCount(codes_size_count, 0, root);
    Clear(root);

    return codes_size_count;
}

std::vector<std::pair<uint16_t, std::string>> GenerateCodes(std::map<uint16_t, int> frequency_map) {
    std::vector<std::pair<uint16_t, int>> sorted_symbols;
    for (auto [symbol, frequency] : frequency_map) {
        sorted_symbols.emplace_back(symbol, frequency);
    } 
    std::sort(sorted_symbols.begin(), sorted_symbols.end(), CompareCodes);
    std::reverse(sorted_symbols.begin(), sorted_symbols.end());

    std::map<uint16_t, uint16_t> codes_size_count = GetCodesSizeCount(frequency_map);

    uint16_t symbols_count = sorted_symbols.size();
    uint16_t symbols_generated_count = 0;

    std::vector<std::string> current_free_codes{"0", "1"};

    std::vector<std::pair<uint16_t, std::string>> codes;

    uint16_t current_symbol_size = 1;
    while (symbols_generated_count != symbols_count) {
        uint16_t count_symbols_current_size = codes_size_count[current_symbol_size];

        for (uint16_t i = 0; i < count_symbols_current_size; ++i) {
            codes.emplace_back(sorted_symbols[symbols_generated_count].first, current_free_codes[i]);
            symbols_generated_count++;
        }

        UpdateCodes(current_free_codes, count_symbols_current_size);
        current_symbol_size++;
    }
    return codes;
}

void Encode(std::string archive_name, std::vector<std::string> filenames) {
    std::ofstream archive_out(archive_name);
    if (!archive_out.is_open()) {
        throw FileDoesNotExist{archive_name};
    }
    BitStream archive_writer{archive_out};

    for (size_t i = 0; i < filenames.size(); ++i) {
        const std::string filename = filenames[i];
        std::map<uint16_t, int> frequency_map;

        frequency_map[FILENAME_END] = 1;
        frequency_map[ONE_MORE_FILE] = 1;
        frequency_map[ARCHIVE_END] = 1;

        std::string filename_wo_path = std::filesystem::path(filename).filename().string();

        for (char c : filename_wo_path) {
            frequency_map[c]++;
        }

        std::ifstream file_in(filename);
        if (!file_in.is_open()) {
            throw FileDoesNotExist{filename};
        }

        char c = 0;
        while (file_in.get(c)) {
            frequency_map[c]++;
        }

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

        for (char c : filename_wo_path) {
            archive_writer.WriteSeq(codes[c]);
        }
        archive_writer.WriteSeq(codes[FILENAME_END]);

        file_in.clear();
        file_in.seekg(0, std::ios::beg);

        while (file_in.get(c)) {
            archive_writer.WriteSeq(codes[c]);
        }

        if (i == filenames.size() - 1) {
            archive_writer.WriteSeq(codes[ARCHIVE_END]);
        } else {
            archive_writer.WriteSeq(codes[ONE_MORE_FILE]);
        }
    }
}

void Decode(std::string archive_name) {
    std::ifstream archive_in(archive_name);

    if (!archive_in.is_open()) {
        throw FileDoesNotExist{archive_name};
    }

    BitStream archive_reader{archive_in};

    while (true) {
        std::map<std::string, uint16_t> codes;

        uint16_t symbols_count = ForceGetFromReader(archive_reader, BITS_IN_ITEM);

        std::vector<uint16_t> symbols(symbols_count);
        for (int i = 0; i < symbols_count; ++i) {
            symbols[i] = ForceGetFromReader(archive_reader, BITS_IN_ITEM);
        }

        uint16_t symbols_read_count = 0;
        std::vector<std::string> current_free_codes{"0", "1"};

        while (symbols_read_count != symbols_count) {
            uint16_t count_symbols_current_size = ForceGetFromReader(archive_reader, BITS_IN_ITEM);

            for (uint16_t i = 0; i < count_symbols_current_size; ++i) {
                codes[current_free_codes[i]] = symbols[symbols_read_count];
                symbols_read_count++;
            }

            UpdateCodes(current_free_codes, count_symbols_current_size);
        }

        uint16_t next_symbol = 0;

        std::string filename;
        while ((next_symbol = GetSymbolFromCodes(codes, archive_reader)) != FILENAME_END) {
            filename += static_cast<char>(next_symbol);
        }

        std::ofstream file_out(filename);

        BitStream file_writer(file_out);

        while (true) {
            next_symbol = GetSymbolFromCodes(codes, archive_reader);
            if (next_symbol == ONE_MORE_FILE) {
                break;
            }
            if (next_symbol == ARCHIVE_END) {
                return;
            }

            file_writer.WriteBits(BYTE_SIZE, static_cast<char>(next_symbol), false);
        }
    }
}

void PrintHelp() {
        std::cout << "Usage: archiver [OPTIONS]\n"
              << "Options:\n"
              << "  -c, --create ARCHIVE_NAME FILE1 [FILE2 ...]  Create an archive with the specified name containing the specified files.\n"
              << "      - ARCHIVE_NAME : Name of the output archive file.\n"
              << "      - FILE1, FILE2, ... : List of files to archive.\n"
              << "  -d, --decompress ARCHIVE_NAME                   Decompress the specified archive, extracting its contents to the current directory.\n"
              << "      - ARCHIVE_NAME : Name of the archive file to decompress.\n"
              << "  -h, --help                                       Display this help message and exit.\n"
              << "Examples:\n"
              << "  archiver -c my_archive.zip file1.txt file2.txt"
              << "  archiver -d my_archive.zip"
              << "  archiver --help"
              << "Note: Ensure that you have permission to read the input files and write to the desired output location.\n";
}

int main(int argc, char* argv[]) {
    try {
        ArgsParser parser{argc, argv};

        if (parser.HasArg(ENCODE_FLAG)) {
            std::vector<std::string> args = parser.GetArgValues(ENCODE_FLAG);
            if (args.empty()) {
                throw ArchiveNameNotPassed{};
            }

            std::string archive_name = args[0];
            if (args.size() < 2) {
                throw FileNameNotPassed{};
            }

            std::vector<std::string> file_names{args.begin() + 1, args.end()};

            Encode(archive_name, file_names);
            
        } else if (parser.HasArg(DECODE_FLAG)) {
            std::vector<std::string> args = parser.GetArgValues(DECODE_FLAG);
            if (args.empty()) {
                throw ArchiveNameNotPassed{};
            }
            std::string archive_name = args[0];

            Decode(archive_name);
        } else if (parser.HasArg(HELP_FLAG)) {
            PrintHelp();
        } else {
            throw InvalidCommandLineArguments{};
        }
    } catch (std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return FAILED_EXIT_CODE;
    }

    return SUCCESS_EXIT_CODE;
}