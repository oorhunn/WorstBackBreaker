//
// Created by Anıl Orhun Demiroğlu on 26.07.2024.
//

#ifndef WORSTBACKBREAKER_THREAD_POOL_HPP
#define WORSTBACKBREAKER_THREAD_POOL_HPP

#include <functional>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

namespace tp {

    struct TaskQueue {

        std::queue<std::function<void()>> m_tasks;
        std::mutex m_mutex;
        std::atomic<std::uint32_t> m_remaining_tasks{0};

        template<typename TCallback>
        auto add_task(TCallback &&callback) -> void {

            std::lock_guard<std::mutex> lock_guard{m_mutex};
            m_tasks.push(std::forward<TCallback>(callback));
            m_remaining_tasks++;
        }

        auto get_task(std::function<void()> &target_callback) -> void {

            std::lock_guard<std::mutex> lock_guard{m_mutex};
            if (m_tasks.empty()) {

                return;
            }
            target_callback = std::move(m_tasks.front());
            m_tasks.pop();
        }

        static auto wait() -> void {

            std::this_thread::yield();
        }

        auto wait_for_completion() -> void const {

            while (m_remaining_tasks > 0) {

                wait();
            }
        }

        auto work_done() -> void {

            m_remaining_tasks--;
        }
    };

    struct Worker {

        std::uint32_t m_id{0};
        std::thread m_thread;
        std::function<void()> m_task{nullptr};
        bool m_running{true};
        TaskQueue *m_queue{nullptr};

        Worker() = default;

        Worker(TaskQueue &queue, std::uint32_t id) :
                m_id{id}, m_queue{&queue} {

            m_thread = std::thread([this]() {
                run();
            });
        }

        auto run() -> void {

            while (m_running) {

                m_queue->get_task(m_task);
                if (m_task == nullptr) {

                    TaskQueue::wait();
                } else {

                    m_task();
                    m_queue->work_done();
                    m_task = nullptr;
                }
            }
        }

        auto stop() -> void {

            m_running = false;
            m_thread.join();
        }
    };

    struct ThreadPool {

        std::uint32_t m_thread_count{0};
        TaskQueue m_queue;
        std::vector<Worker> m_workers;

        explicit ThreadPool(std::uint32_t thread_count) : m_thread_count{thread_count} {

            m_workers.reserve(thread_count);
            for (std::uint32_t i{thread_count}; i--;) {

                m_workers.emplace_back(m_queue, static_cast<std::uint32_t>(m_workers.size()));
            }
        }

        virtual ~ThreadPool() {

            for (Worker &worker: m_workers) {

                worker.stop();
            }
        }

        template<typename TCallBack>
        auto add_task(TCallBack &&callback) -> void {

            m_queue.add_task(std::forward<TCallBack>(callback));
        }

        auto wait_for_completion() -> const void {

            m_queue.wait_for_completion();
        }

        template<typename TCallBack>
        auto dispatch(std::uint32_t element_count, TCallBack &&callback) -> void {

            const std::uint32_t batch_size = element_count / m_thread_count;

            for (std::uint32_t i{0}; i < m_thread_count; i++) {

                const std::uint32_t start = batch_size * i;
                const std::uint32_t end = start + batch_size;
                add_task([start, end, &callback]() { callback(start, end); });
            }

            if (batch_size * m_thread_count < element_count) {

                const std::uint32_t start = batch_size * m_thread_count;
                callback(start, element_count);
            }
            wait_for_completion();
        }
    };
}

#endif //WORSTBACKBREAKER_THREAD_POOL_HPP
