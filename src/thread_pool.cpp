#include "thread_pool.h"

ThreadPool::ThreadPool(std::size_t numThreads) {
    if (numThreads == 0) {
        throw std::invalid_argument(
            "ThreadPool must have at least one worker"
        );
    }

    try {
        workers.reserve(numThreads);

        for (std::size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back(&ThreadPool::workerLoop, this);
        }
    } catch (...) {
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            stopping = true;
        }

        condition.notify_all();

        for (std::thread& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }

        throw;
    }
}

ThreadPool::~ThreadPool() {
    shutdown();
}

void ThreadPool::workerLoop() {
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(queueMutex);

            condition.wait(lock, [this]() {
                return stopping || !tasks.empty();
            });

            if (stopping && tasks.empty()) {
                return;
            }

            task = std::move(tasks.front());
            tasks.pop();
        }

        // Execute the task without holding the queue mutex.
        task();
    }
}

void ThreadPool::shutdown() {
    {
        std::lock_guard<std::mutex> lock(queueMutex);
        stopping = true;
    }

    condition.notify_all();

    for (std::thread& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

std::size_t ThreadPool::workerCount() const {
    return workers.size();
}