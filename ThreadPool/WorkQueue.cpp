
#include "ThreadPool.hpp"
#include "WorkQueue.hpp"

constexpr auto THREAD_POOL_SIZE = 4;
constexpr auto WORK_QUEUE_SIZE = 10;

typedef internal::ThreadPool Pool;

class WorkQueue::Impl {
public:
    Impl()
    : thread_pool(std::make_unique<Pool>(THREAD_POOL_SIZE, WORK_QUEUE_SIZE)) { }
    ~Impl() = default;

    auto enqueue(Task task) -> bool { return thread_pool->enqueue(task); }
    auto start() -> bool { return thread_pool->start(); }
    auto stop() -> bool { return thread_pool->stop(); }

private:
    std::unique_ptr<Pool> thread_pool;
};

WorkQueue::WorkQueue() : _impl(std::make_unique<Impl>()) { }
WorkQueue::~WorkQueue() = default;

auto WorkQueue::enqueue(Task task) -> bool { return _impl->enqueue(task); }
auto WorkQueue::start() -> bool { return _impl->start(); }
auto WorkQueue::stop() -> bool { return _impl->stop(); }
