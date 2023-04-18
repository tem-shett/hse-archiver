set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping -fprofile-arcs -ftest-coverage")

add_catch(test_unit test_priority_queue.cpp ../priority_queue.h test_binary_trie.cpp ../binary_trie.cpp test_bit_reader.cpp ../bit_reader.cpp test_bit_writer.cpp ../bit_writer.cpp)
add_catch(test_priority_queue test_priority_queue.cpp ../priority_queue.h)
add_catch(test_binary_trie test_binary_trie.cpp ../binary_trie.cpp)
add_catch(test_bit_reader test_bit_reader.cpp ../bit_reader.cpp)
add_catch(test_bit_writer test_bit_writer.cpp ../bit_writer.cpp)
