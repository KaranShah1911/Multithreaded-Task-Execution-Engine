#include "thread_pool.h"

#include <atomic>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

void require(bool condition, const std::string& message) {
    if (!condition) {
        throw std::runtime_error("Test failed: " + message);
    }
}

void testWorkerCount() {
    ThreadPool pool(3);

    require(
        pool.workerCount() == 3,
        "Worker count should be three"
    );

    std::cout << "[PASS] Worker count\n";
}

void testTaskResultsAndArguments() {
    ThreadPool pool(2);

    auto result = pool.submit(
        [](int a, int b) {
            return a + b;
        },
        20,
        22
    );

    require(
        result.get() == 42,
        "Submitted task should return 42"
    );

    std::cout << "[PASS] Task results and arguments\n";
}

void testManyTasks() {
    ThreadPool pool(4);

    constexpr int NUM_TASKS = 500;

    std::vector<std::future<int>> results;
    results.reserve(NUM_TASKS);

    for (int i = 0; i < NUM_TASKS; ++i) {
        results.push_back(
            pool.submit([i]() {
                return i * i;
            })
        );
    }

    long long actualSum = 0;

    for (auto& result : results) {
        actualSum += result.get();
    }

    long long expectedSum = 0;

    for (int i = 0; i < NUM_TASKS; ++i) {
        expectedSum += 1LL * i * i;
    }

    require(
        actualSum == expectedSum,
        "All 500 task results should be correct"
    );

    std::cout << "[PASS] Execution of 500 tasks\n";
}

void testExceptionPropagation() {
    ThreadPool pool(2);

    auto result = pool.submit([]() -> int {
        throw std::runtime_error("Expected test exception");
    });

    bool exceptionCaught = false;

    try {
        result.get();
    } catch (const std::runtime_error& error) {
        exceptionCaught =
            std::string(error.what()) == "Expected test exception";
    }

    require(
        exceptionCaught,
        "Task exceptions should propagate through future"
    );

    // The worker should remain usable after a task throws.
    auto nextResult = pool.submit([]() {
        return 100;
    });

    require(
        nextResult.get() == 100,
        "Worker should execute tasks after a task throws"
    );

    std::cout << "[PASS] Exception propagation\n";
}

void testGracefulShutdown() {
    ThreadPool pool(4);

    constexpr int NUM_TASKS = 100;

    std::atomic<int> completed{0};

    std::vector<std::future<void>> results;
    results.reserve(NUM_TASKS);

    for (int i = 0; i < NUM_TASKS; ++i) {
        results.push_back(
            pool.submit([&completed]() {
                completed.fetch_add(1);
            })
        );
    }

    // Shutdown must drain queued tasks before returning.
    pool.shutdown();

    for (auto& result : results) {
        result.get();
    }

    require(
        completed.load() == NUM_TASKS,
        "All queued tasks should complete during shutdown"
    );

    // Submissions after shutdown should be rejected.
    bool rejected = false;

    try {
        auto unused = pool.submit([]() {
            return 1;
        });
        (void)unused;
    } catch (const std::runtime_error&) {
        rejected = true;
    }

    require(
        rejected,
        "Task submission after shutdown should be rejected"
    );

    // Repeated shutdown should be safe.
    pool.shutdown();

    std::cout << "[PASS] Graceful shutdown\n";
}

void testZeroWorkersRejected() {
    bool rejected = false;

    try {
        ThreadPool pool(0);
    } catch (const std::invalid_argument&) {
        rejected = true;
    }

    require(
        rejected,
        "Creating a pool with zero workers should throw"
    );

    std::cout << "[PASS] Zero-worker validation\n";
}

int main() {
    try {
        testWorkerCount();
        testTaskResultsAndArguments();
        testManyTasks();
        testExceptionPropagation();
        testGracefulShutdown();
        testZeroWorkersRejected();

        std::cout << "\nAll tests passed!\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "\n" << error.what() << '\n';
        return 1;
    }
}