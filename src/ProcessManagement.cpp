#include "ProcessManagement.hpp"
#include "Cryption.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/types.h>

ProcessManagement::ProcessManagement() {}

bool ProcessManagement::submitToQueue(std::unique_ptr<Task> task) {
    taskQueue.push(std::move(task));
    pid_t child = fork();
    if (child < 0) {
        std::cout << "Fork failed" << std::endl;
        return false;
    }
    else if (child > 0) {
        std::cout << "Parent process"<< std::endl;
    }
    else {
        std::cout << "Child process"<< std::endl;
        executeTasks();
        std::cout << "Exit the child processes"<< std::endl;
    }
    return true;
}

void ProcessManagement::executeTasks() {
    while (!taskQueue.empty()) {
        std::unique_ptr<Task> taskToExecute = std::move(taskQueue.front());
        taskQueue.pop();

        std::cout << "Executing task: " << taskToExecute->toString() << std::endl;
        executeCryption(taskToExecute->toString());
    }
}
