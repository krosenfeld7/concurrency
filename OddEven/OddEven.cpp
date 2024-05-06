
#include "OddEven.hpp"

void OddEven::print(size_t threadId, unsigned currentCount) const {
    std::cout << "Thread: " << threadId << " current count: " << currentCount << std::endl;
}

void OddEven::startCounting() {
    std::thread threadOdd(&OddEven::incrementOdd, this);
    std::thread threadEven(&OddEven::incrementEven, this);

    threadOdd.join();
    threadEven.join();
}
