# Multithreaded Task Execution Engine

A lightweight task execution engine built with C++17 that uses a fixed-size thread pool, a synchronized task queue, and standard C++ concurrency primitives to execute tasks efficiently.

## Features

- **Fixed-Size Thread Pool:** Reuses worker threads to avoid creating a new thread for every task.
- **Thread-Safe Task Submission:** Uses a mutex-protected queue to safely manage tasks submitted by multiple callers.
- **Condition-Variable Scheduling:** Keeps worker threads idle when no tasks are available and wakes them when new work arrives.
- **Generic Task Execution:** Supports callable objects, lambda expressions, function arguments, and return values.
- **Asynchronous Result Handling:** Uses `std::future` to retrieve task results without directly managing worker threads.
- **Exception Propagation:** Makes task exceptions available through the corresponding futures.
- **Graceful Shutdown:** Stops accepting new tasks, completes queued work, and joins worker threads.
- **Automated Testing:** Includes tests for worker configuration, task execution, result retrieval, exception handling, and shutdown behavior.

## Technology Stack

- **Language:** C++17
- **Standard Library:** STL
- **Concurrency:** `std::thread`, `std::mutex`, `std::condition_variable`
- **Task Management:** `std::packaged_task`, `std::future`
- **Build System:** CMake
- **Testing:** CTest

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
```