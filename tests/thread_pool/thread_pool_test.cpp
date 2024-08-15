//
// Created by Anıl Orhun Demiroğlu on 27.07.2024.
//

#include <gtest/gtest.h>
#include "thread_pool.hpp"  // Replace with the actual path to your thread pool header

TEST(ThreadPoolTest, TaskExecution) {
    tp::ThreadPool threadPool(4);
    std::atomic<int> counter{0};

    auto task = [&counter]() {
        counter++;
    };

    for (int i = 0; i < 10; ++i) {
        threadPool.add_task(task);
    }

    threadPool.wait_for_completion();
    EXPECT_EQ(counter.load(), 10);
}

TEST(ThreadPoolTest, ThreadPoolSize) {
    tp::ThreadPool threadPool(4);
    EXPECT_EQ(threadPool.m_workers.size(), 4);
}


TEST(ThreadPoolTest, TaskOrder) {
    tp::ThreadPool threadPool(2);
    std::vector<int> results;
    std::mutex results_mutex;

    auto task = [&results, &results_mutex](int start, int end) {
        for (int i = start; i < end; ++i) {
            std::lock_guard<std::mutex> lock(results_mutex);
            results.push_back(i);
        }
    };

    threadPool.dispatch(10, task);
    std::vector<int> expected{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    // Sort the results to ensure they match the expected order
    std::sort(results.begin(), results.end());

    EXPECT_EQ(results, expected);
}

TEST(ThreadPoolTest, TaskQueueEmptyHandling) {
    tp::ThreadPool threadPool(4);
    std::atomic<int> counter{0};

    auto task = [&counter]() {
        counter++;
    };

    // Add and execute tasks
    for (int i = 0; i < 5; ++i) {
        threadPool.add_task(task);
    }

    threadPool.wait_for_completion();

    // Ensure that after completing tasks, no additional tasks are left
    EXPECT_EQ(counter.load(), 5);
    EXPECT_EQ(threadPool.m_queue.m_remaining_tasks.load(), 0);
}