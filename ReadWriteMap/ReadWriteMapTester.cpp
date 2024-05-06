
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <unordered_set>
#include <vector>

#include "ReadWriteMap.hpp"

void testMap(std::shared_ptr<ReadWriteMap<int, std::string>> readWriteMap, 
             unsigned testStart,
             unsigned testEnd,
             unsigned tester) {
    std::vector<int> keys;
    std::vector<std::string> values;
    auto threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());

    std::string testThreadId = "Tester " + std::to_string(tester) + 
        ": Starting w/ id: " + std::to_string(threadId) + "\n";

    std::cout << testThreadId << std::endl;

    for (unsigned i = testStart; i < testEnd; i++) {
        keys.push_back(i);
        values.push_back("value" + std::to_string(i));
    }

    unsigned loopCount = std::min(keys.size(), values.size());
    bool success = true;
    std::string successString = "success";

    for (unsigned i = 0; i < loopCount; i++) {
        bool result = readWriteMap->insert(keys[i], values[i]);
        if (!result) {
            success &= result;
            successString = "failed";
        }
    }
    std::cout << "Tester " << tester << ": Inserting into map: " 
        << successString << std::endl;

    success = true;
    successString = "success";
    for (unsigned i = 0; i < loopCount; i++) {
        auto value = readWriteMap->lookup(keys[i]);
        bool result = (value != std::nullopt && *value == values[i]);
        if (!result) {
            success &= result;
            successString = "failed";
        }
    }
    std::cout << "Tester " << tester << ": Finding in map via lookup(): " 
        << successString << std::endl;

    success = true;
    successString = "success";
    for (unsigned i = 0; i < loopCount; i++) {
        auto value = (*readWriteMap)[keys[i]];
        bool result = (value != std::nullopt && *value == values[i]);
        if (!result) {
            success &= result;
            successString = "failed";
        }
    }
    std::cout << "Tester " << tester << ": Finding in map via []: " 
        << successString << std::endl;

    success = true;
    successString = "success";
    for (unsigned i = 0; i < loopCount; i++) {
        bool result = readWriteMap->remove(keys[i]);
        if (!result) {
            success &= result;
            successString = "failed";
        }
    }
    std::cout << "Tester " << tester << ": Deleting keys from map: " 
        << successString << std::endl;

    success = true;
    successString = "success";
    for (unsigned i = 0; i < loopCount; i++) {
        auto value = readWriteMap->lookup(keys[i]);
        bool result = (value == std::nullopt);
        if (!result) {
            success &= result;
            successString = "failed";
        }
    }
    std::cout << "Tester " << tester << 
        ": Finding in map after deletion (success = no entries found): " 
        << successString << std::endl;
}

int main() {
    auto readWriteMap = std::make_shared<ReadWriteMap<int, std::string>>();
    unsigned concurrentTesters = 20;
    unsigned testCountPerThread = 20;
    unsigned start = 0;

    std::cout << "Concurrent Testers: " << concurrentTesters << ", with "
        << testCountPerThread << " tests per thread" << std::endl;

    std::vector<std::thread> threads;

    for (unsigned i = 0; i < concurrentTesters; i++) {
        unsigned end = start + testCountPerThread;
        threads.emplace_back(&testMap, readWriteMap, start, end, i + 1);
        start = end;
    }

    // wait on the running testers
    for (auto& thread : threads) {
        thread.join();
    }
    threads.clear();
    return 0;
}