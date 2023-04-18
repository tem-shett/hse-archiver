#pragma once

#include <iostream>

#include "exceptions.h"

class BitReader {
public:
    explicit BitReader(std::istream& stream, std::string filename = "stream");

    template <typename T>
    bool Read(T& data, size_t bits) {
        data = 0;
        for (size_t i = 0; i < bits; ++i) {
            auto read_bit_result = ReadBit();
            if (!read_bit_result.second) {
                return false;
            }
            data += read_bit_result.first * (1 << (bits - i - 1));
        }
        return true;
    }

    template <typename T>
    void MustRead(T& data, size_t bits, const std::exception& exception) {
        if (!Read(data, bits)) {
            throw exception;
        }
    }

private:
    std::pair<bool, bool> ReadBit();

    std::istream& stream_;
    std::string filename_;
    unsigned char cur_char_[1];
    size_t used_bits_ = 8;
};