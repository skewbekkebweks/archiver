#include "read_bit_stream.h"

#include <limits>

bool ReadBitStream::ReadBit(bool& bit) {
    if (buffer_bit_count_ == 0) {
        if (!in_.get(reinterpret_cast<char&>(buffer_))) {
            failed_ = true;
        }
        buffer_bit_count_ = std::numeric_limits<unsigned char>::digits;
    }
    bit = (buffer_ >> (std::numeric_limits<unsigned char>::digits - 1)) & 1;
    buffer_ <<= 1;
    buffer_bit_count_--;
    if (failed_) {
        return false;
    } else {
        return true;
    }
}