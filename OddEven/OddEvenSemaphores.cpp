
#include "OddEvenSemaphores.hpp"

void OddEvenSemaphores::incrementEven() {
    auto threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
    while (count < max_count) {
        even_semaphore.acquire();
        if (count % 2 == 0) {
            print(threadId, count++);
            odd_semaphore.release();
        }
    }
}

void OddEvenSemaphores::incrementOdd() {
    auto threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
    while (count < max_count) {
        odd_semaphore.acquire();
        if (count % 2 == 1) {
            print(threadId, count++);
            even_semaphore.release();
        }
    }
}
