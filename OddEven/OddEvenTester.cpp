
#include "OddEvenCVMutex.hpp"
#include "OddEvenSemaphores.hpp"

int main() {
    std::cout << "Counting with condition variables and mutexes:" << std::endl;
    OddEvenCVMutex oddEvenCVMutex{DEFAULT_START, DEFAULT_MAX};
    oddEvenCVMutex.startCounting();

    std::cout << "Counting with semaphores:" << std::endl;
    OddEvenSemaphores oddEvenSemaphores{DEFAULT_START, DEFAULT_MAX};
    oddEvenSemaphores.startCounting();

    return 0;
}