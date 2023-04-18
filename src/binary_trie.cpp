#include "binary_trie.h"

BinaryCode& BinaryCode::operator++() {
    if (code == (1 << size) - 1) {
        ++size;
    }
    ++code;
    return *this;
}

BinaryTrie::BinaryTrie() : root_(new BinaryTrieNode) {
}

BinaryTrie::BinaryTrie(std::shared_ptr<BinaryTrieNode> root) : root_(root) {
}

BinaryTrie::~BinaryTrie() {
}

void BinaryTrie::AddBinaryCode(BinaryCode code) {
    auto vertex = root_;
    for (size_t bit = 0; bit < code.size; ++bit) {
        if ((code.code & (1 << (code.size - bit - 1))) == 0) {
            if (vertex->left_child == nullptr) {
                vertex->left_child = std::shared_ptr<BinaryTrieNode>(new BinaryTrieNode);
            }
            vertex = vertex->left_child;
        } else {
            if (vertex->right_child == nullptr) {
                vertex->right_child = std::shared_ptr<BinaryTrieNode>(new BinaryTrieNode);
            }
            vertex = vertex->right_child;
        }
    }
    vertex->is_terminal = true;
    vertex->letter = code.symbol;
}

std::vector<BinaryCode> BinaryTrie::GetAllBinaryCodes() {
    BinaryCode cur_code;
    cur_code.size = 0;
    std::vector<BinaryCode> binary_codes;
    GetAllBinaryCodes(root_, cur_code, binary_codes);
    return binary_codes;
}

void BinaryTrie::GetAllBinaryCodes(std::shared_ptr<BinaryTrieNode> node, BinaryCode cur_code,
                                   std::vector<BinaryCode>& binary_codes) const {
    if (node->is_terminal) {
        binary_codes.push_back({node->letter, cur_code.size, cur_code.code});
    }
    ++cur_code.size;
    cur_code.code <<= 1;
    if (node->left_child) {
        GetAllBinaryCodes(node->left_child, cur_code, binary_codes);
    }
    if (node->right_child) {
        ++cur_code.code;
        GetAllBinaryCodes(node->right_child, cur_code, binary_codes);
    }
}

BinaryTrieTour::BinaryTrieTour(BinaryTrie* trie) : trie_(trie), cur_node_(trie->root_) {
}

void BinaryTrieTour::Move(bool bit) {
    if (cur_node_ == nullptr) {
        return;
    }
    if (!bit) {
        cur_node_ = cur_node_->left_child;
    } else {
        cur_node_ = cur_node_->right_child;
    }
}

void BinaryTrieTour::ReturnToRoot() {
    cur_node_ = trie_->root_;
}

bool BinaryTrieTour::IsTerminal() const {
    return cur_node_->is_terminal;
}

bool BinaryTrieTour::IsCurrentNodeValid() const {
    return cur_node_ != nullptr;
}

uint16_t BinaryTrieTour::GetSymbol() const {
    return cur_node_->letter;
}