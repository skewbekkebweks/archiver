#pragma once

#include <fstream>
#include <cstdint>
#include "exception.h"

const int INT64_T_SIZE = 64;

class ReadBitStream {
public:
    explicit ReadBitStream(std::istream& input) : in_(input), buffer_(0), buffer_bit_count_(0), failed_(false) {
    }

    bool ReadBits(int bits_cnt, uint64_t& num);

private:
    std::istream& in_;
    uint8_t buffer_;
    int buffer_bit_count_;

    bool failed_;

    bool ReadBit(bool& bit);
};

class TryReadMore64BitsError : public std::exception {
public:
    const char* what() const noexcept override {
        return "You can't read more than 64 bits at a time";
    }
};