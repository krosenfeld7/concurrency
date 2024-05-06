
#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "RateLimiter.hpp"

void sleep(unsigned delay) {
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
}

void testWithDelay(std::unique_ptr<RateLimiter>& rateLimiter,
                   std::vector<Request> requests,
                   unsigned delay) {

    std::cout << "Request / Response " << delay 
        << "ms wait in between calls: " << std::endl;
    for (unsigned i = 0; i < requests.size(); i++) {
        auto response = rateLimiter->forward(requests[i]);
        std::cout << requests[i] << " -> " << response << std::endl;
        sleep(delay);
    }
}

void testRateLimiter(std::unique_ptr<RateLimiter>& rateLimiter, 
                     UserId userId, 
                     EndpointName endpoint, 
                     Body message,
                     unsigned testCount,
                     unsigned minDelay,
                     unsigned maxDelay,
                     unsigned resetDelay,
                     unsigned increment) {
    std::vector<Request> requests;
    for (unsigned i = 0; i < testCount; i++) {
        requests.emplace_back(userId, endpoint, message + std::to_string(i));
    }

    for (unsigned i = minDelay; i <= maxDelay; i += increment) {
        testWithDelay(rateLimiter, requests, i);

        std::cout << "Sleep for " << resetDelay 
            << "ms for all access to be reinstated" << std::endl;
        sleep(resetDelay);
    }
}

int main() {
    unsigned window = 10000; // in ms
    unsigned allowance = 5; // 5 calls per 10 seconds
    auto rateLimiter = RateLimiter::getCustomRateLimiter(allowance, window);
    UserId userId = 1111;

    rateLimiter->toggle();

    testRateLimiter(rateLimiter, 1111, "GET", "hello world", 
                    10, 0, 2000, window, 250);    

    rateLimiter->toggle();

    std::cout << "RateLimiter disabled: " << std::endl;
    testRateLimiter(rateLimiter, 1111, "GET", "hello world", 20, 0, 0, 0, 250);    
    
    return 0;
}