#include <_types/_uint16_t.h>
#include "args_parser.h"
#include "exception.h"
#include "bit_stream.h"
#include "trie.h"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <ostream>
#include <string>
#include <map>
#include <filesystem>
#include <queue>
#include <vector>

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

uint16_t GetSymbolFrom(std::map<std::string, uint16_t> codes, BitStream& bs) {
    std::string s;

    while (!codes.contains(s)) {
        s += (bs.ReadBit() ? '1' : '0');
    }

    return codes[s];
}

std::vector<std::pair<uint16_t, std::string>> GenerateCodes(std::map<uint16_t, int> frequency_map) {
    std::vector<std::pair<uint16_t, int>> sorted_symbols;
    for (auto [symbol, frequency] : frequency_map) {
        sorted_symbols.emplace_back(symbol, frequency);
    } 
    std::sort(sorted_symbols.begin(), sorted_symbols.end(), CompareCodes);
    std::reverse(sorted_symbols.begin(), sorted_symbols.end());

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
    std::map<uint16_t, uint16_t> symbols_size_count;
    FillCodes(symbols_size_count, 0, root);

    uint16_t symbols_count = frequency_map.size();
    uint16_t current_symbols_count = 0;

    std::vector<std::string> current_free_codes{"0", "1"};

    std::vector<std::pair<uint16_t, std::string>> codes;

    int i = 1;
    while (current_symbols_count != symbols_count) {
        uint16_t count_symbols_current_size = symbols_size_count[i];
        for (uint16_t i = 0; i < count_symbols_current_size; ++i) {
            codes.emplace_back(sorted_symbols[current_symbols_count].first, current_free_codes[i]);
            current_symbols_count++;
        }
        std::vector<std::string> next_free_codes;
        for (size_t i = count_symbols_current_size; i < current_free_codes.size(); ++i) {
            next_free_codes.push_back(current_free_codes[i] + "0");
            next_free_codes.push_back(current_free_codes[i] + "1");
        }

        swap(current_free_codes, next_free_codes);
        i++;
    }
    return codes;
}

void Encode(std::string archive_name, std::vector<std::string> file_names) {
    std::map<uint16_t, int> frequency_map;

    frequency_map[FILENAME_END] = 1;
    frequency_map[ONE_MORE_FILE] = 1;
    frequency_map[ARCHIVE_END] = 1;

    for (const std::string& file_name : file_names) {
        for (char c : std::filesystem::path(file_name).filename().string()) {
            frequency_map[c]++;
        }

        std::ifstream file(file_name);

        if (!file.is_open()) {
            throw FileDoesNotExist{file_name};
        }

        char c = 0;
        while (file.get(c)) {
            frequency_map[c]++;
        }
    }

    std::vector<std::pair<uint16_t, std::string>> ordered_codes = GenerateCodes(frequency_map);

    std::map<uint16_t, std::string> codes;
    std::map<size_t, int16_t> count_with_size;

    size_t max_sybmol_code_size = 0;
    for (auto [symbol, code] : ordered_codes) {
        std::cout << code << ' ' << symbol << '\n';
        codes[symbol] = code;
        count_with_size[code.size()]++;
        max_sybmol_code_size = std::max(max_sybmol_code_size, code.size());
    }

    std::ofstream file(archive_name);
    BitStream bs{file};

    bs.WriteBits(BITS_IN_ITEM, ordered_codes.size());
    for (auto [symbol, code] : ordered_codes) {
        bs.WriteBits(BITS_IN_ITEM, symbol);
    }
    for (size_t size = 1; size <= max_sybmol_code_size; ++size) {
        bs.WriteBits(BITS_IN_ITEM, count_with_size[size]);
    }

    for (int i = 0; i < file_names.size(); ++i) {
        if (i > 0) {
            bs.WriteSeq(codes[ONE_MORE_FILE]);
        }
        std::string file_name = file_names[i];
        for (char c : std::filesystem::path(file_name).filename().string()) {
            bs.WriteSeq(codes[c]);
        }

        bs.WriteSeq(codes[FILENAME_END]);

        std::ifstream file(file_name);

        if (!file.is_open()) {
            throw FileDoesNotExist{file_name};
        }

        char c = 0;
        while (file.get(c)) {
            bs.WriteSeq(codes[c]);
        }
    }
    bs.WriteSeq(codes[ARCHIVE_END]);
}

void Decode(std::string archive_name) {
    std::ifstream file(archive_name);

    if (!file.is_open()) {
        throw FileDoesNotExist{archive_name};
    }

    BitStream bs{file};

    while (true) {
        std::map<std::string, uint16_t> codes;

        uint16_t symbols_count = bs.ReadBits(BITS_IN_ITEM);
        std::vector<uint16_t> symbols(symbols_count);
        for (int i = 0; i < symbols_count; ++i) {
            symbols[i] = bs.ReadBits(BITS_IN_ITEM);
        }
        uint16_t current_symbols_count = 0;
        std::vector<std::string> current_free_codes{"0", "1"};
        while (current_symbols_count != symbols_count) {
            uint16_t count_symbols_current_size = bs.ReadBits(BITS_IN_ITEM);
            for (uint16_t i = 0; i < count_symbols_current_size; ++i) {
                codes[current_free_codes[i]] = symbols[current_symbols_count];
                current_symbols_count++;
            }
            std::vector<std::string> next_free_codes;
            for (size_t i = count_symbols_current_size; i < current_free_codes.size(); ++i) {
                next_free_codes.push_back(current_free_codes[i] + "0");
                next_free_codes.push_back(current_free_codes[i] + "1");
            }

            swap(current_free_codes, next_free_codes);
        }

        for (auto [key, value] : codes) {
            std::cout << key << ' ' << value << '\n';
        }

        uint16_t next_symbol = 0;

        std::string filename;
        while ((next_symbol = GetSymbolFrom(codes, bs)) != FILENAME_END) {
            filename += static_cast<char>(next_symbol);
        }

        std::cout << next_symbol << ' ' << FILENAME_END << '\n';

        std::cout << filename << '\n';

        std::ofstream file(filename);

        BitStream out(file);

        while (true) {
            next_symbol = GetSymbolFrom(codes, bs);
            if (next_symbol == ONE_MORE_FILE) {
                break;
            }
            if (next_symbol == ARCHIVE_END) {
                return;
            }

            std::cout << next_symbol << ' ' << static_cast<char>(next_symbol) << '\n';

            out.WriteBits(BYTE_SIZE, static_cast<char>(next_symbol), false);
        }
    }
}

void PrintHelp() {
    std::cout << "Hello world!" << '\n';
}

int main(int argc, char* argv[]) {
    try {
        ArgsParser parser{argc, argv};

        if (parser.HasArg(ENCODE_FLAG)) {
            std::vector<std::string> args = parser.GetArgValues("-c");
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
            std::vector<std::string> args = parser.GetArgValues("-d");
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
