#include "bit_reader.h"

BitReader::BitReader(std::istream& stream, std::string filename) : stream_(stream), filename_(filename) {
}

std::pair<bool, bool> BitReader::ReadBit() {
    if (used_bits_ == 8) {
        if (!stream_.read(reinterpret_cast<char*>(cur_char_), 1)) {
            if (!stream_.eof()) {
                throw UnableToReadFromStream(filename_);
            }
            return {false, false};
        }
        used_bits_ = 0;
    }
    ++used_bits_;
    return {cur_char_[0] & (1 << (7 - (used_bits_ - 1))), true};
}