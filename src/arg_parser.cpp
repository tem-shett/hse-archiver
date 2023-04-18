#include "arg_parser.h"

ArgParser::ArgParser(int argc, char** argv) {
    program_location_ = argv[0];
    for (size_t i = 1; i < argc; ++i) {
        std::string cur_arg = argv[i];
        if (cur_arg[0] == '-') {
            if (cur_arg.size() <= 1) {
                continue;
                std::vector<std::string> option;
            }
            if (cur_arg[1] == '-') {
                full_options_.push_back(cur_arg.substr(2, cur_arg.size()));
            } else {
                options_.push_back(cur_arg[1]);
            }
        } else {
            not_option_args_.push_back(cur_arg);
        }
    }
}

size_t ArgParser::CountOptions() const {
    return options_.size();
};

size_t ArgParser::CountFullOptions() const {
    return full_options_.size();
}

size_t ArgParser::CountNotOptionArgs() const {
    return not_option_args_.size();
}

std::vector<char> ArgParser::GetOptions() const {
    return options_;
}

std::vector<std::string> ArgParser::GetFullOptions() const {
    return full_options_;
}

std::vector<std::string> ArgParser::GetNotOptionArgs() const {
    return not_option_args_;
}