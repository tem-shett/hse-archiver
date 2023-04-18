add_executable(
        archiver
        archiver.cpp
        bit_reader.h priority_queue.h binary_trie.cpp bit_reader.cpp bit_writer.cpp compress.cpp decompress.cpp arg_parser.cpp utils.cpp huffman.cpp)

add_subdirectory(unit_tests)
