#include "IO.hpp"
#include <iostream>

IO::IO(const std::string &filePath) {
    file_stream.open(filePath, std::ios::in | std::ios::out | std::ios::binary);
    if (!file_stream.is_open()) {
        std::cout << "Could not open file " << filePath << std::endl;
    }
}

std::fstream &IO::getFileStream() {
    return file_stream;
}

IO::~IO() {
    if (file_stream.is_open()) file_stream.close();
}
