// DynamicCountingSemaphore.hpp
#pragma once

/*
This class defines a basic synamic counting semaphore with an api that
mirrors std::counting_semaphore.
*/

#include <condition_variable>
#include <mutex>

constexpr auto DEFAULT_COUNT = 5;

class DynamicCountingSemaphore {

    public:
        DynamicCountingSemaphore(int semaphoreCount = DEFAULT_COUNT)
        : count(semaphoreCount)
        { }

        void release() {
            std::unique_lock<std::mutex> lock(mutex);
            count++;
            cv.notify_one();
        }

        void acquire() {
            std::unique_lock<std::mutex> lock(mutex);
            cv.wait(lock, [this]() {
                return this->count > 0;
            });
            count--;
        }

        auto try_acquire() -> bool {
            std::unique_lock<std::mutex> lock(mutex, std::try_to_lock);
            if (lock.owns_lock() && count > 0) {
                count--;
                return true;
            }
            return false;
        }

    private:
        int count;
        std::mutex mutex;
        std::condition_variable cv;
};