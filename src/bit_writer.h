#pragma once

#include <iostream>

class BitWriter {
public:
    explicit BitWriter(std::ostream& stream);

    ~BitWriter();

    void Write(uint64_t data, size_t bits);

    void Flash();

private:
    void WriteBit(bool bit);

    std::ostream& stream_;
    unsigned char cur_char_ = 0;
    size_t used_bits_ = 0;
};
