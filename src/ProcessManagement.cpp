#include "ProcessManagement.hpp"
#include "Cryption.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <atomic>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <cstring>
#include <semaphore.h>
#include <mutex>
#include <sys/wait.h>

ProcessManagement::ProcessManagement() {

    startTime = std::chrono::system_clock::now();

    itemsSemaphore = sem_open("/items_semaphore", O_CREAT, 0666, 0);

    if (itemsSemaphore == SEM_FAILED) {
        perror("sem_open for items_semaphore failed");
        exit(EXIT_FAILURE);
    }

    emptySemaphore = sem_open("/empty_slots_semaphore", O_CREAT, 0666, 1000);
    if (emptySemaphore == SEM_FAILED) {
        perror("sem_open for empty_slots_semaphore failed");
        exit(EXIT_FAILURE);
    }

    shmFd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shmFd < 0) {
        perror("shm_open failed");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(shmFd, sizeof(SharedMemory)) < 0) {
        perror("ftruncate failed");
        exit(EXIT_FAILURE);
    }

    sharedMem = static_cast<SharedMemory *>(mmap(nullptr, sizeof(SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0));
    if (sharedMem == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    sharedMem->front = 0;
    sharedMem->rear = 0;
    sharedMem->size.store(0);
}


bool ProcessManagement::submitToQueue(std::unique_ptr<Task> task) {
    sem_wait(emptySemaphore);
    std::unique_lock<std::mutex> lock(queuelock);

    if (sharedMem->size.load() >= 1000) {
        return false;
    }

    strcpy(sharedMem->tasks[sharedMem->rear], task->toString().c_str());
    sharedMem->rear = (sharedMem->rear + 1) % 1000;
    sharedMem->size.fetch_add(1);

    lock.unlock();
    sem_post(itemsSemaphore);

    pid_t child = fork();
    if (child < 0) {
        std::cout << "Fork failed" << std::endl;
        return false;
    } 
    else if (child == 0) {  
        if (sharedMem->size.load() % 500 == 0 || sharedMem->size.load() == 1) {
            std::cout << "Child process executing task: " << task->toString() << std::endl;
        }
        executeTasks();
        _exit(0);
    } 
    else {
        std::cout << "Parent process continuing, child process PID: " << child << std::endl;
    }

    return true;
}



void ProcessManagement::executeTasks() {
    if (sharedMem->size.load() == 0) {
        return;
    }

    sem_wait(itemsSemaphore);
    std::unique_lock<std::mutex> lock(queuelock);
    
    char taskStr[256];
    strcpy(taskStr, sharedMem->tasks[sharedMem->front]);

    sharedMem->front = (sharedMem->front + 1) % 1000;
    sharedMem->size.fetch_sub(1);

    lock.unlock();
    sem_post(emptySemaphore);

    executeCryption(taskStr);
}


ProcessManagement::~ProcessManagement() {
    auto endTime = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    std::cout << "Total time taken for all tasks: " << duration << " ms" << std::endl;
    munmap(sharedMem, sizeof(SharedMemory));
    shm_unlink(SHM_NAME);
}

