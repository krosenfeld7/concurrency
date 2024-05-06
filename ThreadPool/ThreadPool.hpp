#pragma once

#include <memory>
#include <queue>
#include <thread>
#include <vector>

#include "TaskTypeDef.hpp"

namespace internal {
    class ThreadPool {
    public:
        ThreadPool(unsigned threadCount, unsigned queueSize);
        ~ThreadPool();

        /*
         * Adds a task to the queue if the queue is active and isn't full.
         */
        auto enqueue(Task task) -> bool;
        
        /*
         * Activates the queue and sets up the pool.
         */
        auto start() -> bool;

        /*
         * Shuts down the queue and pool.
         */
        auto stop() -> bool;

    private:
        /*
         * Core worker thread loop.
         */
        void monitorQueue();
        auto queueFull() -> bool { return tasks.size() == max_queue_size; }
    
        unsigned max_threads;
        std::atomic<bool> active;
        std::vector<std::unique_ptr<std::thread>> threads;
        std::queue<Task> tasks;
        std::mutex queue_mtx;
        std::condition_variable condition;
        unsigned max_queue_size;
    };
}