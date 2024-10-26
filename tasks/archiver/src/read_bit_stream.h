#pragma once

#include <fstream>
#include <cstdint>
#include "exception.h"

const int INT16_T_SIZE = 16;

class ReadBitStream {
public:
    explicit ReadBitStream(std::istream& input) : in_(input), buffer_(0), buffer_bit_count_(0), failed_(false) {
    }

    bool ReadBits(int bits_cnt, uint16_t& num);

private:
    std::istream& in_;
    uint8_t buffer_;
    int buffer_bit_count_;

    bool failed_;

    bool ReadBit(bool& bit);
};

class TryReadMore16BitsError : public std::exception {
public:
    const char* what() const noexcept override {
        return "You can't read more than 16 bits at a time";
    }
};