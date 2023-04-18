#include "utils.h"

std::string GetFileName(const std::string& file_path) {
    size_t index = file_path.size() - 1;
    while (true) {
        if (file_path[index] == '/') {
            return file_path.substr(index + 1, file_path.size());
        }
        if (index == 0) {
            return file_path;
        }
        --index;
    }
}