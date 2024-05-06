#include <chrono>

#include "RateLimiter.hpp"

#include "APIEndpoints.hpp"

void RateLimiter::toggle() {
    active = !active;
    if (active) {
        startTokenCounter();
    } else {
        stopTokenCounter();
    }
}

auto RateLimiter::forward(Request request) -> Response {
    auto userId = request.getUserId();

    if (!active || allowed(userId)) {
        decrementTokenBalance(userId);
        auto endpoint = APIEndpoints::getEndpoint(request.getDestination());
        return endpoint(userId, request.getBody());
    }
    return Response{userId, "LIMIT EXCEEDED, PLEASE WAIT"};
}

auto RateLimiter::allowed(UserId userId) -> bool {
    auto balance = getTokenBalance(userId);
    if (balance < 0) {
        addUserToLimiter(userId);
        balance = getTokenBalance(userId);
    }
    return balance > 0;
}

void RateLimiter::decrementTokenBalance(UserId userId) {
    std::lock_guard<std::shared_mutex> lock(mutex);

    auto iterator = tokens.find(userId);
    if (iterator != tokens.end()) {
        iterator->second = iterator->second - 1;
    }
}

auto RateLimiter::getTokenBalance(UserId userId) -> TokenCount {
    std::shared_lock<std::shared_mutex> lock(mutex);

    auto iterator = tokens.find(userId);
    if (iterator != tokens.end()) {
        return iterator->second;
    }
    return -1;
}

void RateLimiter::addUserToLimiter(UserId userId) {
    std::lock_guard<std::shared_mutex> lock(mutex);
    tokens.insert({ userId, policy->getAllowance() });
}

void RateLimiter::updateTokenCounts() {
    unsigned tokenUpdateWindow = policy->getWindow() / policy->getAllowance();

    while (active) {
        // update the token count for all users in the tokens map
        {
            std::lock_guard<std::shared_mutex> lock(mutex);

            for (auto& entry : tokens) {
                if (entry.second < policy->getAllowance()) {
                    entry.second += 1;
                }
            }
        }
        auto sleepUntil = std::chrono::system_clock::now() 
            + std::chrono::milliseconds(tokenUpdateWindow);
        std::this_thread::sleep_until(sleepUntil);
    }
}

void RateLimiter::startTokenCounter() {
    backgroundTask = std::thread(&RateLimiter::updateTokenCounts, this);
}

void RateLimiter::stopTokenCounter() {
    active = false;
    backgroundTask.join();
}
