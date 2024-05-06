#pragma once

/*
Provides a generic Dictionary class with basic read/write locking implemented.
Improvements:
- Support for pointers to objects stored
- Locking striping for faster writes
*/

#include <shared_mutex>
#include <unordered_map>
#include <vector>

constexpr auto DEFAULT_SIZE = 5;

template <class Key, class Value>
class Dictionary {
public:
    Dictionary(unsigned capacity = DEFAULT_SIZE)
    : capacity(capacity)
    { }
    ~Dictionary() { }

    /*
     * Inserts the key-value pair into the dictionary if
     * if doesn't already exist and their is space.
     * Will block if another update is in progress.
     */
    auto insert(Key key, Value value) -> bool;
    
    /*
     * Removes the key-value pair from the dictionary if
     * if exists.
     * Will block if another update is in progress.
     */
    auto remove(Key key) -> bool;

    /*
     * Retrieves the value for key from the dictionary if
     * if it exists. 
     */
    auto lookup(Key key) const -> std::optional<Value>;

    /*
     * Retrieves all of the keys from the dictionary. 
     */
    auto getKeys() const -> std::vector<Key>;

    /*
     * Retrieval operator overload
     */
    auto operator[](Key key) const -> std::optional<Value>;

private:
    std::unordered_map<Key, Value> dict;
    unsigned capacity;
    mutable std::shared_mutex mutex;

    /*
     * Helper for determining if the dictionary is empty.
     */
    auto empty() const -> bool;

    /*
     * Helper for determining if the dictionary is full.
     */
    auto full() const -> bool;
};

template <class Key, class Value>
auto Dictionary<Key, Value>::empty() const -> bool { 
    return dict.empty();
}

template <class Key, class Value>
auto Dictionary<Key, Value>::full() const -> bool { 
    return dict.size() == capacity; 
}

template <class Key, class Value>
auto Dictionary<Key, Value>::insert(Key key, Value value) -> bool {
    if (lookup(key) != std::nullopt || full()) { return false; }

    std::unique_lock<std::shared_mutex> lock(mutex);
    dict.insert({ std::move(key), std::move(value) });
    return true;
}

template <class Key, class Value>
auto Dictionary<Key, Value>::remove(Key key) -> bool {
    if (empty()) { return false; }

    std::unique_lock<std::shared_mutex> lock(mutex);
    return dict.erase(key) > 0;
}

template <class Key, class Value>
auto Dictionary<Key, Value>::lookup(Key key) const 
-> std::optional<Value> {
    if (empty()) { return std::nullopt; }

    std::shared_lock<std::shared_mutex> lock(mutex);

    auto value = dict.find(key);
    if (value != dict.end()) {
        return value->second;
    }
    return std::nullopt;
}

template <class Key, class Value>
auto Dictionary<Key, Value>::getKeys() const -> std::vector<Key> {
    std::vector<Key> keys;
    if (empty()) { return keys; }
    {
        std::shared_lock<std::shared_mutex> lock(mutex);

        for (auto& pair : dict) {
            keys.push_back(pair.first);
        }
    }
    std::sort(keys.begin(), keys.end());
    return keys;
}

template <class Key, class Value>
auto Dictionary<Key, Value>::operator[](Key key) const 
-> std::optional<Value> {
    return lookup(key);
}
