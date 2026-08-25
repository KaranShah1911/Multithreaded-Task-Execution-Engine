#include "thread_pool.h"

#include <chrono>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
    constexpr std::size_t NUM_WORKERS = 4;

    ThreadPool pool(NUM_WORKERS);

    std::cout << "Multithreaded Task Execution Engine\n";
    std::cout << "Worker threads: " << pool.workerCount() << "\n\n";

    std::vector<std::future<int>> results;

    // Submit tasks to the thread pool.
    for (int i = 1; i <= 8; ++i) {
        results.push_back(
            pool.submit([i]() {
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(100)
                );

                const int result = i * i;

                std::cout
                    << "Task " << i
                    << " executed by thread "
                    << std::this_thread::get_id()
                    << ", result = " << result
                    << '\n';

                return result;
            })
        );
    }

    // Collect results from all tasks.
    int total = 0;

    for (auto& result : results) {
        total += result.get();
    }

    std::cout << "\nSum of task results: " << total << '\n';

    pool.shutdown();

    std::cout << "All tasks completed. Thread pool shut down.\n";

    return 0;
}