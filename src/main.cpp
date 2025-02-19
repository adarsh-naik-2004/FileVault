#include <iostream>
#include <filesystem>
#include "ProcessManagement.hpp"

int main() {
    std::string directory, actionStr;
    std::cout << "Enter directory path: ";
    std::getline(std::cin, directory);
    std::cout << "Enter action (encrypt/decrypt): ";
    std::getline(std::cin, actionStr);

    if (!std::filesystem::exists(directory) || !std::filesystem::is_directory(directory)) {
        std::cout << "Invalid directory." << std::endl;
        return 1;
    }

    ProcessManagement processManager;
    Action action = (actionStr == "encrypt") ? Action::ENCRYPT : Action::DECRYPT;
    for (const auto &entry : std::filesystem::recursive_directory_iterator(directory)) {
        std::string filePath = entry.path().string();
        IO io(filePath);
        std::fstream f_stream = std::move(io.getFileStream());
        if (f_stream.is_open()) {
            auto task = std::make_unique<Task>(std::move(f_stream), action, filePath);
            processManager.submitToQueue(std::move(task));
        } else {
            std::cout << "Skipping unreadable file " << filePath << std::endl;
        }
    }
    processManager.executeTasks();
    return 0;
}
