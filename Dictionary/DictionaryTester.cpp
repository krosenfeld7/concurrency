
#include "DictionaryTester.hpp"

int main() {
    DictionaryTester<std::string, std::string> tester{10};
    auto testCapacity = 10;

    std::vector<std::string> keys;
    std::vector<std::string> values;

    auto keyPrefix = "Word";
    auto valuePrefix = "Definition";
    for (auto i = 0; i <= testCapacity; ++i) {
        keys.push_back(keyPrefix + std::to_string(i));
        values.push_back(valuePrefix + std::to_string(i));
    }

    tester.setTestKeys(keys);
    tester.setTestValues(values);

    tester.printResults(tester.insertionTest(), 
        DictionaryTester<std::string, std::string>::testName(Tests::INSERT));
    tester.printRetrievalResults(tester.retrievalTest());
    tester.printResults(tester.getKeysTest(), 
        DictionaryTester<std::string, std::string>::testName(Tests::GET_KEYS));
    tester.printResults(tester.deletionTest(), 
        DictionaryTester<std::string, std::string>::testName(Tests::DELETE));
    tester.printResults(tester.getKeysTest(), 
        DictionaryTester<std::string, std::string>::testName(Tests::GET_KEYS));
    tester.printRetrievalResults(tester.retrievalTest());
    tester.printResults(tester.deletionTest(), 
        DictionaryTester<std::string, std::string>::testName(Tests::DELETE));

    return 0;
}