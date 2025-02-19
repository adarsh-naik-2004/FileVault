#include "Task.hpp"
#include <sstream>
#include <stdexcept>

Task::Task(std::fstream &&stream, Action act, std::string filePath)
    : f_stream(std::move(stream)), action(act), filepath(std::move(filePath)) {}

std::string Task::toString() const {
    return filepath + "," + (action == Action::ENCRYPT ? "ENCRYPT" : "DECRYPT");
}

Task Task::fromString(const std::string &taskData) {
    std::istringstream iss(taskData);
    std::string filepath, actionStr;
    if (std::getline(iss, filepath, ',') && std::getline(iss, actionStr)) {
        Action action = (actionStr == "ENCRYPT" ? Action::ENCRYPT : Action::DECRYPT);
        IO io(filepath);
        std::fstream f_stream = std::move(io.getFileStream());
        if (f_stream.is_open()) {
            return Task(std::move(f_stream), action, filepath);
        }
    }
    throw std::runtime_error("Invalid task format");
}