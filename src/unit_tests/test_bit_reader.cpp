#include <catch.hpp>

#include <exception>
#include <random>
#include <sstream>

#include "../exceptions.h"
#include "../bit_reader.h"

template <typename T>
void TestCase1(size_t max_bits = 13) {
    std::mt19937 rnd(777);
    std::string str = "aboba kek\n lol\n 12 %3 1@!@$ SVSDF@#R!#23r232 dsf\n";
    std::istringstream istream(str);
    BitReader bit_reader(istream);
    std::vector<bool> real_value;
    for (unsigned char c : str) {
        for (size_t i = 0; i < 8; ++i) {
            real_value.push_back(c & (1 << (7 - i)));
        }
    }
    std::vector<bool> bit_reader_value;
    size_t count_bits = str.size() * 8;
    while (count_bits > 0) {
        size_t bits = rnd() % max_bits + 1;
        if (bits > count_bits) {
            bits = count_bits;
        }
        T data = 0;
        REQUIRE(bit_reader.Read(data, bits));
        for (size_t i = 0; i < bits; ++i) {
            bit_reader_value.push_back(data & (1 << (bits - 1 - i)));
        }
        count_bits -= bits;
    }
    REQUIRE(real_value == bit_reader_value);
    T data = 0;
    REQUIRE(!bit_reader.Read(data, 1));
}

TEST_CASE("reader1") {
    TestCase1<int16_t>();
    TestCase1<uint16_t>();
    TestCase1<int32_t>();
    TestCase1<uint32_t>();
    TestCase1<unsigned char>(8);
    TestCase1<int64_t>();
    TestCase1<uint64_t>();
}

template <typename T>
void TestCase2() {
    std::string str = "asdlgjlasdg\n  lJSDLG sdfjldj SDFJlKS \n fw12408 * 9f*(DUF)u ()D SD0";
    std::istringstream istream(str);
    BitReader bit_reader(istream);
    try {
        T data = 0;
        for (size_t i = 0; i < str.size(); ++i) {
            bit_reader.MustRead(data, 8, std::exception());
        }
    } catch (...) {
        REQUIRE(false);
    }
    try {
        T data = 0;
        bit_reader.MustRead(data, 1, std::exception());
        REQUIRE(false);
    } catch (...) {
    }
}

TEST_CASE("reader2") {
    TestCase2<int16_t>();
    TestCase2<uint16_t>();
    TestCase2<int32_t>();
    TestCase2<uint32_t>();
    TestCase2<unsigned char>();
    TestCase2<int64_t>();
    TestCase2<uint64_t>();
}
