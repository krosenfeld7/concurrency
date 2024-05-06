
#include "ReadWriteLock.hpp"

void ReadWriteLock::exclusiveLock() {
    {
        std::lock_guard<std::mutex> lock(request_mutex);
        pending_exclusive_requests++;
    }
    access_mutex.lock();
}

void ReadWriteLock::exclusiveUnlock() {
    access_mutex.unlock();
    {
        std::lock_guard<std::mutex> lock(request_mutex);
        if (pending_exclusive_requests > 0) {
            pending_exclusive_requests--;
        }
        if (pending_exclusive_requests == 0) {
            // no more pending exclusive requests, 
            // notify waiting shared requests
            condition.notify_all();
        }
    }
}

void ReadWriteLock::sharedLock() {
    {
        std::unique_lock<std::mutex> lock(request_mutex);
        condition.wait(lock, [this] {
            return this->pending_exclusive_requests == 0;
        });
    }
    access_mutex.lock_shared();
}

void ReadWriteLock::sharedUnlock() {
    access_mutex.unlock_shared();
}