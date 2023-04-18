#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "exceptions.h"

class ArgParser {
public:
    ArgParser(int argc, char** argv);

    size_t CountOptions() const;

    size_t CountFullOptions() const;

    size_t CountNotOptionArgs() const;

    std::vector<char> GetOptions() const;

    std::vector<std::string> GetFullOptions() const;

    std::vector<std::string> GetNotOptionArgs() const;

private:
    std::string program_location_;
    std::vector<std::string> full_options_;
    std::vector<char> options_;
    std::vector<std::string> not_option_args_;
};