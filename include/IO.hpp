#ifndef IO_HPP
#define IO_HPP

#include <fstream>
#include <string>
class IO {
public:
    explicit IO(const std::string &filePath);
    ~IO();
    std::fstream &getFileStream();
private:
    std::fstream file_stream;
};

#endif