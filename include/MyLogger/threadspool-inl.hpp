// threadspool 类的具体实现

#pragma once

#ifndef MYLOGGER_THREADSPOOL_INL_HPP
#define MYLOGGER_THREADSPOOL_INL_HPP

#ifndef MYLOGGER_THREADSPOOL_HPP
#include "threadspool.hpp"
#endif // MYLOGGER_THREADSPOOL_HPP

#include <utility>

template <typename Func, typename... Args>
void ThreadsPool::addFormatTask(Func&& func, Args&&... args) {
    auto task = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
    {
        std::unique_lock<std::mutex> lock(m_format_mtx);
        m_format_queue.emplace(std::move(task));
        // std::cout << "Add task to format thread queue.\n";
    }
    m_format_condition.notify_one();
}

template <typename Func, typename... Args>
void ThreadsPool::addConsoleOutputTask(Func&& func, Args&&... args) {
    auto task = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
    {
        std::unique_lock<std::mutex> lock(m_console_output_mtx);
        m_console_output_queue.emplace(std::move(task));
        // std::cout << "Add task to console output thread queue.\n";
    }
    m_console_output_condition.notify_one();
}

template <typename Func, typename... Args>
void ThreadsPool::addFileOutputTask(Func&& func, Args&&... args) {
    auto task = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
    {
        std::unique_lock<std::mutex> lock(m_file_output_mtx);
        m_file_output_queue.emplace(std::move(task));
        // std::cout << "Add task to file output thread queue.\n";
    }
    m_file_output_condition.notify_one();
}

inline ThreadsPool::ThreadsPool() : m_stop(false), m_format_stop(false) {
    m_format_thread = std::thread([this] {
        while (true) {
            std::unique_lock<std::mutex> lock(m_format_mtx);
            m_format_condition.wait(lock, [&](void) -> bool { return !m_format_queue.empty() || m_stop; });
            if (m_stop && m_format_queue.empty()) {
                m_format_stop = true;
                return;
            }

            auto task(std::move(m_format_queue.front()));
            m_format_queue.pop();
            lock.unlock();
            task();
        }
    });

    m_console_output_thread = std::thread([this] {
        while (true) {
            std::unique_lock<std::mutex> lock(m_console_output_mtx);
            m_console_output_condition.wait(lock, [&](void) -> bool { return !m_console_output_queue.empty() || m_stop; });
            if (m_stop && m_console_output_queue.empty() && m_format_stop) {
                break;
            }

            if (!m_console_output_queue.empty()) {
                auto task(std::move(m_console_output_queue.front()));
                m_console_output_queue.pop();
                lock.unlock();
                task();
            } else {
                lock.unlock();
            }
        }
    });

    m_file_output_thread = std::thread([this] {
        while (true) {
            std::unique_lock<std::mutex> lock(m_file_output_mtx);
            m_file_output_condition.wait(lock, [&](void) -> bool { return !m_file_output_queue.empty() || m_stop; });
            if (m_stop && m_file_output_queue.empty() && m_format_stop) {
                break;
            }

            if (!m_file_output_queue.empty()) {
                auto task(std::move(m_file_output_queue.front()));
                m_file_output_queue.pop();
                lock.unlock();
                task();
            } else {
                lock.unlock();
            }
        }
    });
}

inline ThreadsPool::~ThreadsPool() {
    {
        std::unique_lock<std::mutex> format_console_lock(m_format_mtx);
        std::unique_lock<std::mutex> output_console_lock(m_console_output_mtx);
        std::unique_lock<std::mutex> output_file_lock(m_file_output_mtx);
        m_stop = true;
    }

    m_format_condition.notify_all();
    m_console_output_condition.notify_all();
    m_file_output_condition.notify_all();

    if (m_format_thread.joinable())
        m_format_thread.join();
    if (m_console_output_thread.joinable())
        m_console_output_thread.join();
    if (m_file_output_thread.joinable())
        m_file_output_thread.join();
}

inline ThreadsPool& ThreadsPool::getThreadsPool() {
    static ThreadsPool instance;
    return instance;
}

#endif // MYLOGGER_THREADSPOOL_INL_HPP
