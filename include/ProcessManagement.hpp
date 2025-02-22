#ifndef PROCESS_MANAGEMENT_HPP
#define PROCESS_MANAGEMENT_HPP

#include "Task.hpp"
#include <queue>
#include <memory>
#include <iostream>
#include <atomic>
#include <semaphore.h>
#include <mutex>

class ProcessManagement {

public:
    ProcessManagement();
    ~ProcessManagement();
    bool submitToQueue(std::unique_ptr<Task> task);
    void executeTasks();

private:
    sem_t* itemsSemaphore;
    sem_t* emptySemaphore;

    std::chrono::time_point<std::chrono::system_clock> startTime;


    struct SharedMemory
    {
        std::atomic<int> size;
        char tasks[1000][256];
        int front;
        int rear;

        void prinSharedMemory() {
            std::cout<< size <<std::endl;
        }
    };
    SharedMemory* sharedMem;

    int shmFd;

    const char* SHM_NAME = "/my_queue";

    std::mutex queuelock;
    
};

#endif
