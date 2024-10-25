#pragma once

#include <fstream>
#include <cstdint>

class WriteBitStream {
public:
    explicit WriteBitStream(std::ostream& output)
        : out_(output), buffer_(0), buffer_bit_count_(0) {
    }

    ~WriteBitStream();

    void WriteBits(int bits_cnt, uint16_t value);

    void WriteSeq(std::string s);

private:
    std::ostream& out_;
    uint8_t buffer_;
    int buffer_bit_count_;

    void WriteBit(bool bit);

    void Flush();
};