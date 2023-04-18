#include "compress.h"

bool ComparatorBinaryCodes(const BinaryCode& lhs, const BinaryCode& rhs) {
    return std::tie(lhs.size, lhs.symbol) < std::tie(rhs.size, rhs.symbol);
}

struct HuffmanSymbol {
    size_t frequency;
    uint16_t symbol;
    std::shared_ptr<BinaryTrieNode> trie_node_ptr;

    bool operator<(const HuffmanSymbol& rhs) const {
        return std::tie(frequency, symbol) < std::tie(rhs.frequency, rhs.symbol);
    }

    bool operator==(const HuffmanSymbol& rhs) const {
        return std::tie(frequency, symbol) == std::tie(rhs.frequency, rhs.symbol);
    }
};

std::vector<BinaryCode> GetHuffmanCodes(std::vector<size_t> frequency) {
    PriorityQueue<HuffmanSymbol> heap;
    for (int16_t i = 0; i <= ARCHIVE_END; ++i) {
        if (frequency[i] > 0) {
            std::shared_ptr<BinaryTrieNode> node(new BinaryTrieNode);
            node->is_terminal = true;
            node->letter = i;
            heap.Insert({frequency[i], node->letter, node});
        }
    }
    while (heap.Size() > 1) {
        auto [freq1, sym1, node1] = heap.Pop();
        auto [freq2, sym2, node2] = heap.Pop();
        std::shared_ptr<BinaryTrieNode> parent_node(new BinaryTrieNode);
        parent_node->left_child = node1;
        parent_node->right_child = node2;
        heap.Insert({freq1 + freq2, std::min(sym1, sym2), parent_node});
    }
    BinaryTrie trie(heap.Pop().trie_node_ptr);
    return trie.GetAllBinaryCodes();
}

void Compress(std::vector<std::string> input_files, std::string output_file) {
    std::ofstream stream_out(output_file, std::ios::out | std::ios::binary);
    BitWriter bit_writer(stream_out);
    for (size_t file_index = 0; file_index < input_files.size(); ++file_index) {
        // get istream
        auto input_file = input_files[file_index];
        auto filename = GetFileName(input_file);
        if (filename == "." || filename.empty()) {
            throw UnableToOpenFile(input_file);
        }
        std::ifstream stream_in(input_file, std::ios::in | std::ios::binary);
        if (!stream_in) {
            throw UnableToOpenFile(input_file);
        }

        // frequency count
        std::vector<size_t> frequency(ARCHIVE_END + 1);
        BitReader bit_reader(stream_in, input_file);
        unsigned char c = 0;
        while (bit_reader.Read(c, 8)) {
            ++frequency[c];
        }
        for (auto sym : filename) {
            ++frequency[sym];
        }
        frequency[FILENAME_END] = frequency[ONE_MORE_FILE] = frequency[ARCHIVE_END] = 1;

        // get Huffman codes
        auto binary_codes = GetHuffmanCodes(frequency);
        std::sort(binary_codes.begin(), binary_codes.end(), ComparatorBinaryCodes);
        auto canonical_codes = RestoreCanonicalForm(binary_codes);

        // write compressed data to archive
        bit_writer.Write(canonical_codes.size(), 9);
        for (auto code : canonical_codes) {
            bit_writer.Write(code.symbol, 9);
        }
        std::vector<size_t> count_code_sizes(binary_codes.back().size + 1);
        std::vector<BinaryCode> code_by_sym(ARCHIVE_END + 1);
        for (auto code : canonical_codes) {
            ++count_code_sizes[code.size];
            code_by_sym[code.symbol] = code;
        }
        for (size_t i = 1; i < count_code_sizes.size(); ++i) {
            bit_writer.Write(count_code_sizes[i], 9);
        }
        for (auto sym : filename) {
            bit_writer.Write(code_by_sym[sym].code, code_by_sym[sym].size);
        }
        bit_writer.Write(code_by_sym[FILENAME_END].code, code_by_sym[FILENAME_END].size);
        std::ifstream stream_in2(input_file, std::ios::in | std::ios::binary);
        if (!stream_in2) {
            throw UnableToOpenFile(input_file);
        }
        BitReader bit_reader2(stream_in2, input_file);
        while (bit_reader2.Read(c, 8)) {
            bit_writer.Write(code_by_sym[c].code, code_by_sym[c].size);
        }
        if (file_index < input_files.size() - 1) {
            bit_writer.Write(code_by_sym[ONE_MORE_FILE].code, code_by_sym[ONE_MORE_FILE].size);
        } else {
            bit_writer.Write(code_by_sym[ARCHIVE_END].code, code_by_sym[ARCHIVE_END].size);
        }
    }
}