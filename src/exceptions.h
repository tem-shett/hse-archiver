#pragma once

#include <exception>
#include <string>

class Exception : public std::exception {
public:
    const char* what() const noexcept override {
        return msg_.c_str();
    }

protected:
    std::string msg_;
};

class WrongCommandFormat : public Exception {
public:
    explicit WrongCommandFormat() {
        msg_ = "Command format is not valid. Use -h for help.";
    }
};

class UnableToOpenFile : public Exception {
public:
    explicit UnableToOpenFile(std::string filename) {
        msg_ = "Unable to open " + filename;
    }
};

class UnableToReadFromStream : public Exception {
public:
    explicit UnableToReadFromStream(std::string filename) {
        msg_ = "Unable to read from " + filename;
    }
};

class CompressedFileIsCorrupted : public Exception {
public:
    explicit CompressedFileIsCorrupted() {
        msg_ = "Compressed file is corrupted";
    }
};
