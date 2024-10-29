#pragma once

#include <fstream>
#include <cstdint>
#include <limits>
#include <type_traits>
#include "exception.h"

class TryReadManyBitsError : public std::exception {
public:
    const char* what() const noexcept override {
        return "You can't read that many bits at a time";
    }
};

class NotIntegerPassedError : public std::exception {
public:
    const char* what() const noexcept override {
        return "You can't pass not an integer type as a num";
    }
};

class ReadBitStream {
public:
    explicit ReadBitStream(std::istream& input) : in_(input), buffer_(0), buffer_bit_count_(0), failed_(false) {
    }

    template <typename T>
    bool ReadBits(int bits_cnt, T& num) {
        if (!std::is_integral<T>::value) {
            throw NotIntegerPassedError{};
        }

        if (bits_cnt > std::numeric_limits<T>::digits) {
            throw TryReadManyBitsError{};
        }
        num = 0;
        bool bit = false;

        for (int i = bits_cnt - 1; i >= 0; --i) {
            if (!ReadBit(bit)) {
                return false;
            }
            num |= (bit << i);
        }

        return true;
    }

private:
    std::istream& in_;
    uint8_t buffer_;
    int buffer_bit_count_;

    bool failed_;

    bool ReadBit(bool& bit);
};