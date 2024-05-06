
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "WorkQueue.hpp"

// A simple task that prints a message to standard output
void printTask(const std::string& message) {
    std::cout << message << std::endl;
}

int main() {
    WorkQueue workQueue;

    // Start the work queue / thread pool
    workQueue.start();

    // Queue several tasks
    for (auto i = 0; i <= 100; i++) {
        workQueue.enqueue([i]() {
            printTask("Task " + std::to_string(i) + 
                      ": Queue and Thread Pool demonstration");
            });
    }
    // Wait a bit for tasks to execute
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Stop the work queue / thread pool
    workQueue.stop();

    return 0;
}
