#include <catch.hpp>

#include <algorithm>
#include <random>

#include "../binary_trie.h"

TEST_CASE("trie") {
    std::mt19937 rnd(23);
    BinaryCode cur_code;
    BinaryTrieNode root;
    BinaryTrie trie(std::make_shared<BinaryTrieNode>(root));
    const size_t count_nodes = 200;
    std::vector<BinaryCode> binary_codes;
    for (size_t i = 0; i < count_nodes; ++i) {
        ++cur_code.symbol;
        size_t delta = rnd() % 1000 + 5;
        for (size_t j = 0; j < delta; ++j) {
            ++cur_code;
        }
        binary_codes.push_back(cur_code);
    }
    auto binary_codes2 = binary_codes;
    std::shuffle(binary_codes2.begin(), binary_codes2.end(), rnd);
    for (auto code : binary_codes2) {
        trie.AddBinaryCode(code);
    }
    auto trie_codes = trie.GetAllBinaryCodes();
    REQUIRE(binary_codes.size() == count_nodes);
    std::sort(trie_codes.begin(), trie_codes.end(),
              [](const BinaryCode& lhs, const BinaryCode& rhs) { return lhs.symbol < rhs.symbol; });
    for (size_t i = 0; i < count_nodes; ++i) {
        auto code1 = binary_codes[i];
        auto code2 = trie_codes[i];
        REQUIRE(code1.symbol == code2.symbol);
        REQUIRE(code1.size == code2.size);
        REQUIRE(code1.code == code2.code);
    }
}

TEST_CASE("trie_tour") {
    std::mt19937 rnd(77);
    BinaryCode cur_code;
    BinaryTrie trie;
    const size_t count_nodes = 200;
    std::vector<BinaryCode> binary_codes;
    for (size_t i = 0; i < count_nodes; ++i) {
        ++cur_code.symbol;
        size_t delta = rnd() % 1000 + 5;
        for (size_t j = 0; j < delta; ++j) {
            ++cur_code;
        }
        trie.AddBinaryCode(cur_code);
        REQUIRE(trie.GetAllBinaryCodes().size() == i + 1);
        binary_codes.push_back(cur_code);
    }
    BinaryTrieTour tour(&trie);
    for (auto code : binary_codes) {
        tour.ReturnToRoot();
        for (size_t b = 0; b < code.size; ++b) {
            REQUIRE(tour.IsCurrentNodeValid());
            tour.Move(code.code & (1LL << (code.size - b - 1)));
        }
        REQUIRE(tour.IsCurrentNodeValid());
        REQUIRE(tour.IsTerminal());
        REQUIRE(tour.GetSymbol() == code.symbol);
    }
    tour.Move(false);
    REQUIRE(!tour.IsCurrentNodeValid());
    tour.Move(true);
    REQUIRE(!tour.IsCurrentNodeValid());
}

TEST_CASE("empty_trie") {
    BinaryTrieNode root;
    BinaryTrie trie(std::make_shared<BinaryTrieNode>(root));
    REQUIRE(trie.GetAllBinaryCodes().empty());
    BinaryTrieTour tour(&trie);
    tour.Move(false);
    REQUIRE(!tour.IsCurrentNodeValid());
}

TEST_CASE("full_trie") {
    BinaryTrie trie;
    size_t code_size = 15;
    for (uint64_t code = 0; code < (1LL << code_size); ++code) {
        BinaryCode cur_code;
        cur_code.size = code_size;
        cur_code.code = cur_code.symbol = code;
        trie.AddBinaryCode(cur_code);
    }
    REQUIRE(trie.GetAllBinaryCodes().size() == (1LL << code_size));
}