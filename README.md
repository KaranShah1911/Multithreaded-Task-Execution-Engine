# Multithreaded Task Execution Engine

A lightweight task execution engine implemented in C++17 using a fixed-size thread pool, a synchronized task queue, and standard C++ concurrency primitives.

## Features

- **Fixed-size thread pool:** Reuses worker threads instead of creating a new thread for every task.
- **Thread-safe task submission:** Multiple callers can submit tasks through a mutex-protected queue.
- **Condition-variable scheduling:** Workers sleep when no tasks are available and wake when work arrives.
- **Generic task execution:** Supports callable objects, lambdas, function arguments, and return values.
- **Asynchronous results:** Uses `std::future` to retrieve results from submitted tasks.
- **Exception propagation:** Task exceptions can be retrieved through their associated futures.
- **Graceful shutdown:** Rejects new submissions and processes queued tasks before joining worker threads.
- **Automated tests:** Covers worker configuration, results, task throughput, exception handling, and shutdown.

## Technology Stack

- C++17
- Standard Template Library (STL)
- `std::thread`
- `std::mutex`
- `std::condition_variable`
- `std::packaged_task`
- `std::future`
- CMake
- CTest

## Project Structure

```text
.
├── include/
│   └── thread_pool.h
├── src/
│   ├── thread_pool.cpp
│   └── main.cpp
├── tests/
│   └── thread_pool_tests.cpp
├── CMakeLists.txt
├── README.md
└── .gitignore
