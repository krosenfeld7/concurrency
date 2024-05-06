#pragma once

#include <atomic>
#include <memory>
#include <shared_mutex>
#include <thread>
#include <unordered_map>

#include "Policy.hpp"
#include "Requests.hpp"

typedef int TokenCount;

class RateLimiter {
public:
    RateLimiter()
    : policy(std::make_shared<Policy>())
    { }

    RateLimiter(unsigned allowance, unsigned window)
    : policy(std::make_shared<Policy>(allowance, window))
    { }

    ~RateLimiter()
    { if (active) { stopTokenCounter(); } }

    /*
     * Clients call this function, requests are forwarded and responses
     * are returned. Rate limited while active is true.
     */
    auto forward(Request request) -> Response;

    /*
     * Enables / disables the rate limiter.
     */
    void toggle();

    /*
     * Default RateLimiter factory, provides default Policy.
     */
    static auto getDefaultRateLimiter() -> std::unique_ptr<RateLimiter> { 
        return std::make_unique<RateLimiter>(); 
    }

    /*
     * Custom allowance and window policy factory.
     */
    static auto getCustomRateLimiter(unsigned allowance, unsigned window) 
    -> std::unique_ptr<RateLimiter> { 
        return std::make_unique<RateLimiter>(allowance, window); 
    }

private:
    /*
     * Kicks off a thread to increment token counts on the policy's interval.
     */
    void startTokenCounter();

    /*
     * Stops the thread that is incrementing token counts.
     */
    void stopTokenCounter();

    /*
     * This function must be called on a background thread only.
     */
    void updateTokenCounts();

    /*
     * Checks if the user is within its allowance.
     */
    auto allowed(UserId userId) -> bool;

    /*
     * Consumes a token for this user. Returns false on failure.
     */
    void decrementTokenBalance(UserId userId);

    /*
     * Begins tracking the user in the rate limiter.
     */
    void addUserToLimiter(UserId userId);

    /*
     * Gets the users token allowance remaining.
     */
    auto getTokenBalance(UserId userId) -> TokenCount;

    std::shared_ptr<Policy> policy;
    std::unordered_map<UserId,  TokenCount> tokens;
    std::atomic<bool> active = false;
    std::thread backgroundTask;
    std::shared_mutex mutex;
};

