#pragma once

#include "TaskTypeDef.hpp"

class WorkQueue {
public:
    WorkQueue();
    ~WorkQueue();

    /*
     * Public facing enqueue function that supports
     * adding to the work queue.
     */
    auto enqueue(Task task) -> bool;

    /*
     * Starts up the work queue.
     */
    auto start() -> bool;

    /*
     * Shuts down the work queue.
     */
    auto stop() -> bool;

private:
    class Impl;
    std::unique_ptr<Impl> _impl;
};
