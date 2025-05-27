#include "write_bit_stream.h"

#include <limits>

WriteBitStream::~WriteBitStream() {
    Flush();
}

void WriteBitStream::WriteBits(int bits_cnt, uint16_t value) {
    for (int i = bits_cnt - 1; i >= 0; --i) {
        WriteBit((value >> i) & 1);
    }
}

void WriteBitStream::WriteSeq(std::string s) {
    for (char c : s) {
        WriteBit((c == '0' ? false : true));
    }
}

void WriteBitStream::WriteBit(bool bit) {
    buffer_ = (buffer_ << 1) | (bit ? 1 : 0);
    buffer_bit_count_++;

    if (buffer_bit_count_ == std::numeric_limits<unsigned char>::digits) {
        out_.put(static_cast<char>(buffer_));
        buffer_ = 0;
        buffer_bit_count_ = 0;
    }
}

void WriteBitStream::Flush() {
    while (buffer_bit_count_ > 0) {
        WriteBit(false);
    }
}