
#include "OddEvenCVMutex.hpp"

void OddEvenCVMutex::incrementEven() {
    auto threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
    while (count < max_count) {
        std::unique_lock<std::mutex> lock(counting_mutex);

        even_variable.wait(lock, [this] { return this->count % 2 == 0; });

        if (!lock.owns_lock() || count % 2 == 1) {
            continue;
        }
        print(threadId, count++);
        odd_variable.notify_one();
    }
}

void OddEvenCVMutex::incrementOdd() {
    auto threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
    while (count < max_count) {
        std::unique_lock<std::mutex> lock(counting_mutex);

        odd_variable.wait(lock, [this] { return this->count % 2 == 1; });

        if (!lock.owns_lock() || count % 2 == 0) {
            continue;
        }
        print(threadId, count++);
        even_variable.notify_one();
    }
}
