#pragma once

/*
Basic policy implementation.
*/

#include <memory>

constexpr auto DEFAULT_ALLOWANCE = 5; // allowed messages per time window
constexpr auto DEFAULT_WINDOW = 10000; // in ms

class Policy {
public:
    Policy(unsigned allowance = DEFAULT_ALLOWANCE, 
           unsigned window = DEFAULT_WINDOW)
    : allowance{allowance}
    , window{window}
    { }
    Policy(Policy& policy)
    : allowance{policy.allowance}
    , window{policy.window}
    { }

    auto getAllowance() -> unsigned { return allowance; }
    auto getWindow() -> unsigned { return window; }

    static auto defaultPolicy() -> std::shared_ptr<Policy> { 
        return std::make_shared<Policy>();
    }

private:
    unsigned allowance;
    unsigned window;
};