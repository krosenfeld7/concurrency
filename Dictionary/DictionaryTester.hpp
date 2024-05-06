#pragma once

/*
Test class for the Dictionary object. Performs all
testing.
*/

#include <iostream>
#include <string>
#include <map>
#include <unordered_set>
#include <vector>

#include "Dictionary.hpp"

enum Tests { INSERT, RETRIEVE, DELETE, GET_KEYS };

template <class Key, class Value>
class DictionaryTester {
public:
    DictionaryTester(unsigned capacity = DEFAULT_SIZE)
    : dict{capacity}
    { }

    auto insertionTest() -> std::map<Key, bool>;
    auto retrievalTest() -> std::map<Key, std::pair<bool, bool>>;
    auto deletionTest() -> std::map<Key, bool>;
    auto getKeysTest() -> std::map<Key, bool>;

    void setTestKeys(std::vector<Key> newKeys) { keys = newKeys; }
    void setTestValues(std::vector<Value> newValues) { values = newValues; }

    void printResults(const std::map<Key, bool>& results, 
                      const std::string testName);
    void printRetrievalResults(const std::map<Key, 
                               std::pair<bool, bool>>& results);

    static auto testName(Tests test) -> std::string;
private:
    Dictionary<Key, Value> dict;
    std::vector<Key> keys;
    std::vector<Value> values;
};

template <class Key, class Value>
auto DictionaryTester<Key, Value>::insertionTest() -> std::map<Key, bool> {
    std::map<Key, bool> results;
    auto loopSize = std::min(keys.size(), values.size());
    for (auto i = 0; i < loopSize; i++) {
        results.insert({keys[i], dict.insert(keys[i], values[i])});
    }
    return results;
}

template <class Key, class Value>
auto DictionaryTester<Key, Value>::retrievalTest()
-> std::map<Key, std::pair<bool, bool>> {
    std::map<Key, std::pair<bool, bool>> results;
    auto loopSize = std::min(keys.size(), values.size());
    for (auto i = 0; i < loopSize; i++) {
        auto funcLookup = dict.lookup(keys[i]);
        auto operatorLookup = dict[keys[i]];

        auto funcResult = 
            (funcLookup != std::nullopt && *funcLookup == values[i]);
        auto operatorResult = 
            (operatorLookup != std::nullopt && *operatorLookup == values[i]);

        results.emplace(keys[i], 
                        std::forward_as_tuple(funcResult, operatorResult));
    }
    return results;
}

template <class Key, class Value>
auto DictionaryTester<Key, Value>::deletionTest() 
-> std::map<Key, bool> {
    std::map<Key, bool> results;
    auto loopSize = std::min(keys.size(), values.size());
    for (auto i = 0; i < loopSize; i++) {
        results.insert({keys[i], dict.remove(keys[i])});
    }
    return results;
}

template <class Key, class Value>
auto DictionaryTester<Key, Value>::getKeysTest() 
-> std::map<Key, bool> {
    std::map<Key, bool> results;
    auto loopSize = std::min(keys.size(), values.size());
    auto dictKeys = dict.getKeys();

    std::unordered_set<Key> keySet(dictKeys.begin(), dictKeys.end());
    for (auto i = 0; i < loopSize; i++) {
        results.insert({keys[i], keySet.find(keys[i]) != keySet.end()});
    }
    return results;
}

template <class Key, class Value>
void DictionaryTester<Key, Value>::printResults(const std::map<Key, bool>& results, 
                                                const std::string testName) {
    auto testNumber = 1;
    for (const auto& result : results) {
        std::cout << testName << " " << testNumber++ << 
            ": " << result.first << "  result: " << result.second << std::endl;
    }
}

template <class Key, class Value>
void DictionaryTester<Key, Value>::printRetrievalResults(const std::map<Key, 
                                                         std::pair<bool, bool>>& results) {
    auto testNumber = 1;
    auto testName = DictionaryTester<Key, Value>::testName(Tests::RETRIEVE);
    for (const auto& result : results) {
        std::cout << testName << " " << testNumber++ << ": " << result.first <<
            "  lookup(): " << std::get<0>(result.second) << ", []: " << 
            std::get<1>(result.second) << std::endl;
    }
}

template <class Key, class Value>
auto DictionaryTester<Key, Value>::testName(Tests test) -> std::string {
    switch(test) {
        case INSERT: return std::string("INSERTION");
        case RETRIEVE: return std::string("RETRIEVE");
        case DELETE: return std::string("DELETE");
        case GET_KEYS: return std::string("GET_KEYS");
        default: return std::string("INVALID TEST");
    }
}