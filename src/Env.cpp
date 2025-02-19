#include "Env.hpp"
#include "IO.hpp"
#include <sstream>

std::string ReadEnv::getEnv() {
    IO io(".env");
    std::fstream f_stream = std::move(io.getFileStream());
    std::stringstream buffer;
    buffer << f_stream.rdbuf();
    return buffer.str();
}