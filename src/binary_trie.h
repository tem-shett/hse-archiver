#pragma once

#include <string>
#include <vector>
#include <memory>

class BinaryTrieNode {
public:
    std::shared_ptr<BinaryTrieNode> left_child = nullptr;
    std::shared_ptr<BinaryTrieNode> right_child = nullptr;
    bool is_terminal = false;
    uint16_t letter;
};

class BinaryCode {
public:
    uint16_t symbol = 0;
    size_t size = 1;
    uint64_t code = 0;

    BinaryCode& operator++();
};

class BinaryTrie {
public:
    friend class BinaryTrieTour;

    BinaryTrie();

    explicit BinaryTrie(std::shared_ptr<BinaryTrieNode> root);

    ~BinaryTrie();

    void AddBinaryCode(BinaryCode code);

    std::vector<BinaryCode> GetAllBinaryCodes();

private:
    void GetAllBinaryCodes(std::shared_ptr<BinaryTrieNode> node, BinaryCode cur_code,
                           std::vector<BinaryCode>& binary_codes) const;

    std::shared_ptr<BinaryTrieNode> root_;
};

class BinaryTrieTour {
public:
    explicit BinaryTrieTour(BinaryTrie* trie);

    void Move(bool bit);

    void ReturnToRoot();

    bool IsTerminal() const;

    bool IsCurrentNodeValid() const;

    uint16_t GetSymbol() const;

private:
    BinaryTrie* const trie_;
    std::shared_ptr<BinaryTrieNode> cur_node_;
};
