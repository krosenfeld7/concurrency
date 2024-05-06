// BoundedHashSet.hpp
#pragma once

/*
This class defines a bounded hash set which is bounded using a counting semaphore.
On add to the set, this class ensures that the counting semaphore limit is not
reached. If it has been, it will fail to add. If it hasn't, it will add the
entry to the set and increment the semaphore. On removal, the semaphore is decremented.
*/

#include <mutex>
#include <unordered_set>

#include "DynamicCountingSemaphore.hpp"

template <class Entry>
class BoundedHashSet {
private:
    DynamicCountingSemaphore boundedSemaphore;
    std::unordered_set<std::shared_ptr<Entry>> hashSet;
    std::mutex mutex;

public:
    BoundedHashSet(int bound)
    : boundedSemaphore{bound}
    { }

    /*
     * Adds an entry to the set if possible. Returns true if added successfully.
     * Non-blocking add.
     */
    auto add(std::shared_ptr<Entry> entry) -> bool {
        if (!boundedSemaphore.try_acquire()) {
            return false;
        }
        std::unique_lock<std::mutex> lock(mutex, std::try_to_lock);
        if (!lock.owns_lock()) {
            boundedSemaphore.release();
            return false;
        }
        auto [_, inserted] = hashSet.insert(entry);
        // insert entry, release semaphore on failure
        if (!inserted) {
            boundedSemaphore.release();
        }
        return inserted;
    }

    /*
     * Removes an entry from the set if it exists. Returns true if removed successfully.
     * Blocking remove.
     */
    auto remove(std::shared_ptr<Entry> entry) -> bool {
        std::unique_lock<std::mutex> lock(mutex);
        auto erased = hashSet.erase(entry) > 0;
        if (erased) {
            boundedSemaphore.release();
        }
        return erased;
    }

    /*
     * Checks if an entry exists in the set.
     * Blocking contains.
     */
    auto contains(std::shared_ptr<Entry> entry) -> bool {
        std::unique_lock<std::mutex> lock(mutex);
        return hashSet.find(entry) != hashSet.end();
    }
};