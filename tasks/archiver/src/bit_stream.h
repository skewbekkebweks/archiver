#include <exception>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <istream>
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

    ~BitStream() {
        Flush();
    }

    void WriteBits(int bits_cnt, uint16_t value, bool is_little_endian = true) {
        if (is_little_endian) {
            for (int i = 0; i <= bits_cnt - 1; ++i) {
                WriteBit((value >> i) & 1);
            }
        } else {
            for (int i = bits_cnt - 1; i >= 0; --i) {
                WriteBit((value >> i) & 1);
            }
        }
    }

    void WriteSeq(std::string s) {
        for (char c : s) {
            WriteBit((c == '0' ? false : true));
        }
    }

    std::optional<uint16_t> ReadBits(int bits_cnt, bool is_little_endian = true) {
        uint16_t value = 0;

        if (is_little_endian) {
            for (int i = 0; i <= bits_cnt - 1; ++i) {
                std::optional<bool> bit = ReadBit();
                if (bit.has_value()) {
                    value |= (bit.value() << i);
                } else {
                    return std::nullopt;
                }
            }
        } else {
            for (int i = bits_cnt - 1; i >= 0; --i) {
                std::optional<bool> bit = ReadBit();
                if (bit.has_value()) {
                    value |= (bit.value() << i);
                } else {
                    return std::nullopt;
                }
            }
        }

        return value;
    }
private:
    std::istream& in_;
    std::ostream& out_;
    uint8_t buffer_in_;
    uint8_t buffer_out_;
    int buffer_bit_count_in_;
    int buffer_bit_count_out_;

    bool failed_;

    void WriteBit(bool bit) {
        buffer_out_ = (buffer_out_ << 1) | (bit ? 1 : 0);
        buffer_bit_count_out_++;

        if (buffer_bit_count_out_ == BYTE_SIZE) {
            out_.put(static_cast<char>(buffer_out_));
            buffer_out_ = 0;
            buffer_bit_count_out_ = 0;
        }
    }

    std::optional<bool> ReadBit() {
        if (buffer_bit_count_in_ == 0) {
            buffer_in_ = in_.get();
            buffer_bit_count_in_ = BYTE_SIZE;
            if (in_.eof() || in_.fail()) {
                failed_ = true;
            }
        }
        bool bit = (buffer_in_ >> (BYTE_SIZE - 1)) & 1;
        buffer_in_ <<= 1;
        buffer_bit_count_in_--;
        if (failed_) {
            return std::nullopt;
        } else {
            return bit;
        }
    }

    void Flush() {
        while (buffer_bit_count_out_ > 0) {
            WriteBit(false);
        }
    }
};
