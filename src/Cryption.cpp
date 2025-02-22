#include "Cryption.hpp"
#include "Task.hpp"
#include "Env.hpp"
#include <iostream>
#include <ctime>
#include <iomanip>
int executeCryption(const std::string &taskData) {
    Task task = Task::fromString(taskData);
    ReadEnv env;
    int key = std::stoi(env.getEnv());

    char ch;
    while (task.f_stream.get(ch)) {
        ch = (task.action == Action::ENCRYPT) ? (ch + key) % 256 : (ch - key + 256) % 256;
        task.f_stream.seekp(-1, std::ios::cur);
        task.f_stream.put(ch);
    }
    task.f_stream.close();

    std::time_t curr_time = std::time(nullptr);

    std::tm* now = std::localtime(&curr_time);

    std::cout <<"Exit at:"<< std::put_time(now, "%Y-%m-%d %H:%M:%S")<< std::endl;

    return 0;
}