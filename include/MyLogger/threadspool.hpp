// 线程池类

#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>

class ThreadsPool {
  private:
    friend class Logger;

    // 包含三组线程和任务列表，三组线程分别负责格式化、输出到控制台、输出到文件
  private:
    std::mutex m_format_mtx;
    std::condition_variable m_format_condition;
    std::thread m_format_thread;
    std::queue<std::function<void(void)>> m_format_queue;

    std::mutex m_console_output_mtx;
    std::condition_variable m_console_output_condition;
    std::thread m_console_output_thread;
    std::queue<std::function<void(void)>> m_console_output_queue;

    std::mutex m_file_output_mtx;
    std::condition_variable m_file_output_condition;
    std::thread m_file_output_thread;
    std::queue<std::function<void(void)>> m_file_output_queue;

    bool m_stop;
    bool m_format_stop;

  private:
    ThreadsPool();
    ~ThreadsPool();
    ThreadsPool(const ThreadsPool&) = delete;
    ThreadsPool& operator=(const ThreadsPool&) = delete;
    static ThreadsPool& getThreadsPool();

    template <typename Func, typename... Args>
    void addFormatTask(Func&& func, Args&&... args);

    template <typename Func, typename... Args>
    void addConsoleOutputTask(Func&& func, Args&&... args);

    template <typename Func, typename... Args>
    void addFileOutputTask(Func&& func, Args&&... args);

    template <typename Func, typename... Args>
    void addTask(bool console, bool file, Func&& func, Args&&... args);
};

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
