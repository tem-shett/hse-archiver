#include <iostream>

#include "arg_parser.h"
#include "compress.h"
#include "decompress.h"
#include "exceptions.h"

void PrintHelpMessage() {
    std::cout << "Usage: archiver [options] [archive_name] [file_names]\n"
                 "    -h                                         help message\n"
                 "    -c archive_name file1 [file2 ...]          compress files 'file1', 'file2', ... to archive "
                 "'archive_name'\n"
                 "    -d archive_name                            decompress archive 'archive_name'\n"
              << std::endl;
}

int main(int argc, char** argv) {
    try {
        ArgParser arg_parser(argc, argv);
        if (arg_parser.CountFullOptions() > 0 || arg_parser.CountOptions() != 1) {
            throw WrongCommandFormat();
        }
        char flag = arg_parser.GetOptions()[0];
        if (flag == 'c') {
            if (arg_parser.CountNotOptionArgs() < 2) {
                throw WrongCommandFormat();
            }
            auto filenames = arg_parser.GetNotOptionArgs();
            Compress(std::vector<std::string>(filenames.begin() + 1, filenames.end()), filenames[0]);
            return 0;
        }
        if (flag == 'd') {
            if (arg_parser.CountNotOptionArgs() != 1) {
                throw WrongCommandFormat();
            }
            Decompress(arg_parser.GetNotOptionArgs()[0]);
            return 0;
        }
        if (flag == 'h') {
            if (arg_parser.CountNotOptionArgs() > 0) {
                throw WrongCommandFormat();
            }
            PrintHelpMessage();
            return 0;
        }
    } catch (WrongCommandFormat& e) {
        std::cout << e.what() << std::endl;
        return 111;
    } catch (UnableToOpenFile& e) {
        std::cout << e.what() << std::endl;
        return 111;
    } catch (UnableToReadFromStream& e) {
        std::cout << e.what() << std::endl;
        return 111;
    } catch (CompressedFileIsCorrupted& e) {
        std::cout << e.what() << std::endl;
        return 111;
    }
    return 0;
}
