#include "decompress.h"

uint16_t GetNextSymbol(BinaryTrieTour& trie_tour, BitReader& bit_reader, const std::exception& exception) {
    trie_tour.ReturnToRoot();
    bool bit = false;
    while (trie_tour.IsCurrentNodeValid() && !trie_tour.IsTerminal()) {
        bit_reader.MustRead(bit, 1, exception);
        trie_tour.Move(bit);
    }
    if (!trie_tour.IsCurrentNodeValid()) {
        throw exception;
    }
    return trie_tour.GetSymbol();
}

void Decompress(std::string input_file) {
    std::ifstream stream_in(input_file, std::ios::in | std::ios::binary);
    if (!stream_in) {
        throw UnableToOpenFile(input_file);
    }
    BitReader bit_reader(stream_in, input_file);
    CompressedFileIsCorrupted corrupted_file_exception = CompressedFileIsCorrupted();
    while (true) {
        // get canonical codes
        size_t symbols_count = 0;
        bit_reader.MustRead(symbols_count, 9, corrupted_file_exception);
        if (symbols_count > ARCHIVE_END + 1) {
            throw corrupted_file_exception;
        }
        std::vector<BinaryCode> canonical_codes(symbols_count);
        for (size_t i = 0; i < symbols_count; ++i) {
            bit_reader.MustRead(canonical_codes[i].symbol, 9, corrupted_file_exception);
        }
        std::vector<size_t> count_code_sizes{0};
        size_t index = 0;
        while (index != symbols_count) {
            count_code_sizes.push_back(0);
            bit_reader.MustRead(count_code_sizes.back(), 9, corrupted_file_exception);
            if (index + count_code_sizes.back() > symbols_count) {
                throw corrupted_file_exception;
            }
            for (size_t i = 0; i < count_code_sizes.back(); ++i) {
                canonical_codes[index].size = count_code_sizes.size() - 1;
                ++index;
            }
        }
        canonical_codes = RestoreCanonicalForm(canonical_codes);

        // restore compressed data
        BinaryTrie trie;
        for (auto code : canonical_codes) {
            trie.AddBinaryCode(code);
        }
        BinaryTrieTour trie_tour(&trie);
        std::string output_file;
        uint16_t sym = 0;
        do {
            sym = GetNextSymbol(trie_tour, bit_reader, corrupted_file_exception);
            if (sym != FILENAME_END) {
                output_file.push_back(static_cast<char>(sym));
            }
        } while (sym != FILENAME_END);
        std::ofstream stream_out(output_file, std::ios::out | std::ios::binary);
        BitWriter bit_writer(stream_out);
        do {
            sym = GetNextSymbol(trie_tour, bit_reader, corrupted_file_exception);
            if (sym != ONE_MORE_FILE && sym != ARCHIVE_END) {
                bit_writer.Write(sym, 8);
            }
        } while (sym != ONE_MORE_FILE && sym != ARCHIVE_END);
        if (sym == ARCHIVE_END) {
            break;
        }
    }
}