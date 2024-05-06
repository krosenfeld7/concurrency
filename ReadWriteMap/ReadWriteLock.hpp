#pragma once

/*
This ReadWriteLock provides access restrictions to a
resource. It prevents write starvation by queueing the
count and prevent further write access when there are pending
writes. Read access is unrestricted as long as the resource
is not exclusively locked. 
*/

#include <condition_variable>
#include <mutex>
#include <shared_mutex>

class ReadWriteLock {
public:
    /*
     * Requests exclusive access to the resource.
     * Will block until access is granted. Tracks the
     * number of pending exclusive requests to prevent
     * starvation.
     */
    void exclusiveLock();

    /*
     * Unlocks exclusive access to the resource.
     * Updates the exclusive request count and notifies
     * all waiting shared requests if no more exclusive
     * requests are waiting.
     */
    void exclusiveUnlock();

    /*
     * Requests shared access to the resource.
     * Blocks until exclusive requests are complete,
     * but will be given immediate access afterwards.
     */
    void sharedLock();

    /*
     * Unlocks shared access to the resource. 
     * Straightforward and simple.
     */
    void sharedUnlock();

private:
    std::mutex request_mutex;
    std::shared_mutex access_mutex;
    std::condition_variable condition;
    unsigned pending_exclusive_requests = 0;
};
