#pragma once

#include <fstream>
#include <iostream>
#include <cstdint>
#include <vector>
#include <optional>

const int BYTE_SIZE = 8;


class BitStream {
public:
    explicit BitStream() 
        : in_(std::cin), out_(std::cout), buffer_in_(0), buffer_out_(0), buffer_bit_count_in_(0), buffer_bit_count_out_(0), failed_(false) {
    }

    explicit BitStream(std::ostream& output)
        : in_(std::cin), out_(output), buffer_in_(0), buffer_out_(0), buffer_bit_count_in_(0), buffer_bit_count_out_(0), failed_(false) {
    }

    explicit BitStream(std::istream& input, std::ostream& output = std::cout)
        : in_(input), out_(output), buffer_in_(0), buffer_out_(0), buffer_bit_count_in_(0), buffer_bit_count_out_(0), failed_(false) {
    }

    ~BitStream();

    void WriteBits(int bits_cnt, uint16_t value, bool is_little_endian = true);

    void WriteSeq(std::string s);

    std::optional<uint16_t> ReadBits(int bits_cnt, bool is_little_endian = true);

private:
    std::istream& in_;
    std::ostream& out_;
    uint8_t buffer_in_;
    uint8_t buffer_out_;
    int buffer_bit_count_in_;
    int buffer_bit_count_out_;

    bool failed_;

    void WriteBit(bool bit);

    std::optional<bool> ReadBit();

    void Flush();
};
