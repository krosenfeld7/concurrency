# concurrency
C++ Concurrent Data Structures. Implemented for personal use / understanding ins and outs of C++ concurrency.

Includes:
- Bounded set, which is restricted using semaphores
- Dictionary, which supports key-value lookups, insertions, deletions
- RateLimiter, provides a basic API Rate Limiter that is configurable
- ReadWriteMap, uses lock striping to support concurrent safe reading, while protecting data integrity while reading and writing
- ThreadPool, a basic work queue pool

- OddEven, an attempt at solving the OddEven problem using semaphores and mutexes
