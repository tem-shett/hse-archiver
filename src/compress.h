#pragma once

#include <algorithm>
#include <cstdint>
#include <map>
#include <vector>
#include <tuple>
#include <fstream>

#include "archive_constants.h"
#include "bit_reader.h"
#include "bit_writer.h"
#include "binary_trie.h"
#include "exceptions.h"
#include "huffman.h"
#include "priority_queue.h"
#include "utils.h"

void Compress(std::vector<std::string> input_files, std::string output_file);
