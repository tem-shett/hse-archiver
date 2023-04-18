#include <catch.hpp>

#include <sstream>
#include <random>

#include "../bit_writer.h"

TEST_CASE("writer1") {
    std::mt19937 rnd(179);
    for (size_t _ = 0; _ < 10; ++_) {
        std::ostringstream ostream;
        std::string real_data;
        {
            BitWriter bit_writer(ostream);
            std::vector<bool> data;
            size_t bytes = 100;
            for (size_t i = 0; i < 8 * bytes; ++i) {
                data.push_back(rnd() % 2);
            }
            size_t count_bits = 8 * bytes;
            size_t index = 0;
            while (count_bits > 0) {
                size_t bits = rnd() % 15 + 1;
                if (bits > count_bits) {
                    bits = count_bits;
                }
                uint64_t cur_data = 0;
                for (size_t i = 0; i < bits; ++i) {
                    cur_data += data[index] * (1 << (bits - i - 1));
                    ++index;
                }
                bit_writer.Write(cur_data, bits);
                count_bits -= bits;
            }
            for (size_t b = 0; b < bytes; ++b) {
                unsigned char cur_data = 0;
                for (size_t i = 0; i < 8; ++i) {
                    cur_data += data[b * 8 + i] * (1 << (7 - i));
                }
                real_data.push_back(static_cast<char>(cur_data));
            }
        }
        REQUIRE(real_data == ostream.str());
    }
}

TEST_CASE("writer2") {
    std::ostringstream ostream;
    { BitWriter bit_writer(ostream); }
    REQUIRE(ostream.str().empty());
}

TEST_CASE("writer3") {
    std::ostringstream ostream;
    std::string real_data;
    BitWriter bit_writer(ostream);
    bit_writer.Write(0, 1);
    bit_writer.Flash();
    real_data += static_cast<char>(0);
    REQUIRE(real_data == ostream.str());
    bit_writer.Flash();
    REQUIRE(real_data == ostream.str());
    bit_writer.Write(0, 1);
    bit_writer.Flash();
    real_data += static_cast<char>(0);
    REQUIRE(real_data == ostream.str());
}