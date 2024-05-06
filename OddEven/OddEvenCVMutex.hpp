#pragma once

#include <condition_variable>
#include <mutex>

#include "OddEven.hpp"

class OddEvenCVMutex : public OddEven {
public:
    using super = OddEven;

    OddEvenCVMutex(unsigned startCount = DEFAULT_START, unsigned max = DEFAULT_MAX)
    : count(startCount)
    , max_count(max)
    { }
    ~OddEvenCVMutex()
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
    mutable std::mutex counting_mutex;
    std::condition_variable even_variable;
    std::condition_variable odd_variable;
};