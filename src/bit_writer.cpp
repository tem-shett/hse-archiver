#include "bit_writer.h"

BitWriter::BitWriter(std::ostream& stream) : stream_(stream) {
}

BitWriter::~BitWriter() {
    Flash();
}

void BitWriter::WriteBit(bool bit) {
    if (used_bits_ == 8) {
        stream_ << cur_char_;
        cur_char_ = 0;
        used_bits_ = 0;
    }
    cur_char_ += bit * (1 << (7 - used_bits_));
    ++used_bits_;
}

void BitWriter::Write(uint64_t data, size_t bits) {
    for (size_t i = 0; i < bits; ++i) {
        WriteBit(data & (1 << (bits - i - 1)));
    }
}

void BitWriter::Flash() {
    if (used_bits_ > 0) {
        stream_ << cur_char_;
        used_bits_ = 0;
        cur_char_ = 0;
    }
}