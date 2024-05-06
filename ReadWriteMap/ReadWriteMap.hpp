#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "ReadWriteLock.hpp"

constexpr auto DEFAULT_BUCKET_COUNT = 5;

template <class K, class V>
class ReadWriteMap {
public:
    ReadWriteMap(unsigned buckets = DEFAULT_BUCKET_COUNT);
    ~ReadWriteMap();

    /*
     * Inserts the key-value pair into the map.
     * Uses hash() to determine the bucket.
     * Requires exclusive access to the bucket.
     */
    auto insert(K key, V value) -> bool;

    /*
     * Removes the key-value pair into the map.
     * Uses hash() to determine the bucket.
     * Requires exclusive access to the bucket.
     */
    auto remove(K key) -> bool;

    /*
     * Same as above, ignores the provided parameter.
     */
    auto remove(K key, V value) -> bool;

    /*
     * Finds the key-value pair in the map.
     * Uses hash() to determine the bucket.
     * Only needs shared access to the bucket.
     */
    auto lookup(K key) -> std::optional<V>;

    /*
     * Finds the key-value pair in the map using the
     * lookup() function.
     */
    auto operator[](K key) -> std::optional<V>;

private:
    std::vector<std::unique_ptr<ReadWriteLock>> locks;
    std::vector<std::unique_ptr<std::unordered_map<K, V>>> buckets;
    //std::mutex access_mutex;
    unsigned bucket_count;

    /*
     * Uses std::hash() to determine a hash for the
     * provided key.
     */
    auto hash(K key) const -> unsigned;

    /*
     * Provides the corresponding bucket for the key
     * via hash() above. 
     */
    auto getBucketIndex(K key) const -> unsigned;
};

template <class K, class V>
ReadWriteMap<K, V>::ReadWriteMap(unsigned bucketCount)
: bucket_count{bucketCount} {
    for (auto i = 0; i < bucket_count; i++) {
        locks.emplace_back(std::make_unique<ReadWriteLock>());
        buckets.emplace_back(std::make_unique<std::unordered_map<K, V>>());
    }
}

template <class K, class V>
ReadWriteMap<K, V>::~ReadWriteMap() {
    locks.clear();
    buckets.clear();
}

template <class K, class V>
auto ReadWriteMap<K, V>::hash(K key) const -> unsigned {
    return std::hash<K>{}(key);
}

template <class K, class V>
auto ReadWriteMap<K, V>::getBucketIndex(K key) const -> unsigned {
    return hash(key) % bucket_count;
}

template <class K, class V>
auto ReadWriteMap<K, V>::insert(K key, V value) -> bool {
    auto bucketIndex = getBucketIndex(key);
    // verify the bucket index is valid
    if (bucketIndex >= locks.size() || bucketIndex >= buckets.size()) {
        return false;
    }
    // request exclusive access to the specific bucket
    locks[bucketIndex]->exclusiveLock();
    auto [_, inserted] = buckets[bucketIndex]->insert({ key, value });
    // relinquish access to the bucket
    locks[bucketIndex]->exclusiveUnlock();

    return inserted;
}

template <class K, class V>
auto ReadWriteMap<K, V>::remove(K key) -> bool {
    auto bucketIndex = getBucketIndex(key);
    // verify the bucket index is valid
    if (bucketIndex >= locks.size() || bucketIndex >= buckets.size()) {
        return false;
    }
    // request exclusive access to the specific bucket
    if (bucketIndex )
    locks[bucketIndex]->exclusiveLock();
    auto erased = buckets[bucketIndex]->erase(key) > 0;
    // relinquish access to the bucket
    locks[bucketIndex]->exclusiveUnlock();

    return erased;
}

template <class K, class V>
auto ReadWriteMap<K, V>::remove(K key, V value) -> bool {
    return remove(key);
}

template <class K, class V>
auto ReadWriteMap<K, V>::lookup(K key) -> std::optional<V> {
    auto bucketIndex = getBucketIndex(key);
    // verify the bucket index is valid
    if (bucketIndex >= locks.size() || bucketIndex >= buckets.size()) {
        return std::nullopt;
    }
    // request shared access to the specific bucket
    locks[bucketIndex]->sharedLock();

    auto valueIterator = buckets[bucketIndex]->find(key);
    std::optional<V> result = std::nullopt;
    if (valueIterator != buckets[bucketIndex]->end()) {
        result = valueIterator->second;
    }
    // relinquish access to the bucket
    locks[bucketIndex]->sharedUnlock();
    return result;
}

template <class K, class V>
auto ReadWriteMap<K, V>::operator[](K key) -> std::optional<V> {
    return lookup(key);
}
