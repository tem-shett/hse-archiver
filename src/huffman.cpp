#include "huffman.h"

std::vector<BinaryCode> RestoreCanonicalForm(const std::vector<BinaryCode>& binary_codes) {
    std::vector<BinaryCode> canonical_codes;
    BinaryCode cur_code;
    for (auto code : binary_codes) {
        while (cur_code.size < code.size) {
            ++cur_code.size;
            cur_code.code <<= 1;
        }
        canonical_codes.push_back({code.symbol, cur_code.size, cur_code.code});
        ++cur_code;
    }
    return canonical_codes;
}