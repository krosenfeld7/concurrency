
#include <iostream>

#include "ThreadPool.hpp"

using namespace internal;

ThreadPool::ThreadPool(unsigned threadCount, unsigned queueSize)
: max_threads(threadCount)
, active(false)
, max_queue_size(queueSize)
{ }

ThreadPool::~ThreadPool() { stop(); }

auto ThreadPool::start() -> bool {
    for (auto i = 0; i < max_threads; ++i) {
        threads.emplace_back(std::make_unique<std::thread>(&ThreadPool::monitorQueue, this));
    }
    active = true;
    return active;
}

auto ThreadPool::stop() -> bool {
    {
        std::unique_lock<std::mutex> lock(queue_mtx);
        active = false;
    }
    condition.notify_all();
    for (auto& thread : threads) {
        thread->join();
    }
    threads.clear();
    return !active;
}

auto ThreadPool::enqueue(Task task) -> bool {
    if (!active || queueFull()) { return false; }
    {
        std::unique_lock<std::mutex> lock(queue_mtx);
        tasks.push(std::move(task));
    }
    condition.notify_one();
    return true;
}

void ThreadPool::monitorQueue() {
    while (true) {
        Task task;
        {
            std::unique_lock<std::mutex> lock(queue_mtx);
            condition.wait(lock, [this] {
                return !tasks.empty() || !active;
            });
            if (!active) { return; }
            task = std::move(tasks.front());
            tasks.pop();
        }
        try {
            task();
        } catch (std::exception& exception) {
            std::cerr << "Exception occurred: " << exception.what() 
            << std::endl;
        } catch (...) {
            std::cerr << "Unknown exception occurred" << std::endl;
        }
    }
}
