#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <condition_variable>
#include <cstddef>
#include <future>
#include <functional>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

class ThreadPool {
public:
    explicit ThreadPool(std::size_t numThreads);

    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    template <typename F, typename... Args>
    auto submit(F&& function, Args&&... args)
        -> std::future<
            std::invoke_result_t<F, Args...>
        >
    {
        using ReturnType = std::invoke_result_t<F, Args...>;

        auto task = std::make_shared<
            std::packaged_task<ReturnType()>
        >(
            std::bind(
                std::forward<F>(function),
                std::forward<Args>(args)...
            )
        );

        std::future<ReturnType> result = task->get_future();

        {
            std::lock_guard<std::mutex> lock(queueMutex);

            if (stopping) {
                throw std::runtime_error(
                    "Cannot submit a task to a stopped ThreadPool"
                );
            }

            tasks.emplace([task]() {
                (*task)();
            });
        }

        condition.notify_one();

        return result;
    }

    void shutdown();

    std::size_t workerCount() const;

private:
    void workerLoop();

    std::vector<std::thread> workers;

    std::queue<std::function<void()>> tasks;

    mutable std::mutex queueMutex;
    std::condition_variable condition;

    bool stopping = false;
};

#endif