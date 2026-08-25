cmake_minimum_required(VERSION 3.16)

project(
    MultithreadedTaskExecutionEngine
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_library(thread_pool
    src/thread_pool.cpp
)

target_include_directories(thread_pool
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(thread_pool
    PUBLIC
        Threads::Threads
)

add_executable(task_engine_demo
    src/main.cpp
)

target_link_libraries(task_engine_demo
    PRIVATE
        thread_pool
)

enable_testing()

add_executable(thread_pool_tests
    tests/thread_pool_tests.cpp
)

target_link_libraries(thread_pool_tests
    PRIVATE
        thread_pool
)

add_test(
    NAME thread_pool_tests
    COMMAND thread_pool_tests
)