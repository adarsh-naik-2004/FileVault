#include "ProcessManagement.hpp"
#include "Cryption.hpp"
#include <iostream>

ProcessManagement::ProcessManagement() {}

bool ProcessManagement::submitToQueue(std::unique_ptr<Task> task) {
    taskQueue.push(std::move(task));
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
