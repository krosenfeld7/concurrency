#pragma once

#include <atomic>
#include <iostream>
#include <thread>

constexpr auto DEFAULT_START = 1;
constexpr auto DEFAULT_MAX = 100;

class OddEven {
public:
    OddEven() = default;
    ~OddEven() = default;

    /*
     * Even thread entry, counts evens.
     */
    virtual void incrementEven() = 0;

    /*
     * Odd thread entry, counts odds.
     */
    virtual void incrementOdd() = 0;

    /*
     * Initializes threads for counting and starts them.
     */
    void startCounting();

protected:

    /*
     * Prints to cout
     */
    void print(size_t threadId, unsigned currentCount) const;
};
