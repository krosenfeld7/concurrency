#pragma once

#include <semaphore>

#include "OddEven.hpp"

class OddEvenSemaphores : public OddEven {
public:
    using super = OddEven;

    OddEvenSemaphores(unsigned startCount = DEFAULT_START, unsigned max = DEFAULT_MAX)
    : count(startCount)
    , max_count(max)
    { }
    ~OddEvenSemaphores()
    { }

    /*
     * Even thread entry, counts evens.
     */
    virtual void incrementEven() override;

    /*
     * Odd thread entry, counts odds.
     */
    virtual void incrementOdd() override;

private:
    std::atomic<unsigned> count;
    std::atomic<unsigned> max_count;
    std::binary_semaphore odd_semaphore{1};
    std::binary_semaphore even_semaphore{0};
};