#ifndef TASK_HPP
#define TASK_HPP

#include <string>
#include <fstream>
#include "IO.hpp"

enum class Action { ENCRYPT, DECRYPT };

struct Task {
    std::string filepath;
    std::fstream f_stream;
    Action action;

    Task(std::fstream &&stream, Action act, std::string filePath);
    std::string toString() const;
    static Task fromString(const std::string &taskData);
};

#endif