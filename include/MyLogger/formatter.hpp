// 格式化字符串解析器，所有成员均设置为私有，禁止用户直接访问，只能通过 Logger 类进行操作。

#pragma once

#ifndef MYLOGGER_FORMATTER_HPP
#define MYLOGGER_FORMATTER_HPP

#include <chrono>
#include <string>
#include <utility>
#include <vector>

#include "loglevel.hpp"

class Formatter {
  private:
    // 友元类声明，仅允许 Logger 类访问私有成员
    friend class Logger;

  private:
    // 解析结果的 Token 类型
    enum class Token { LEVEL, TIME, THREAD, TEXT, ARG };

  private:
    LogLevel m_level;                             // 日志等级
    std::chrono::system_clock::time_point m_time; // 时间戳
    std::string m_thread_id;                      // 线程ID

    // 格式化字符串解析结果
    std::vector<std::pair<Token, std::string>> m_format_tokens;

  private:
    // 构造函数设为私有，禁止用户直接构造对象
    explicit Formatter(LogLevel level);

  private:
    // 解析单个格式化字符串，返回 Token 和内容。仅由 tokenize() 调用。
    std::pair<Token, std::string> tokenize(const std::string& token_string);

    // 将 m_format_tokens 转换为格式化后的字符串，用于输出结果
    std::string formatedString() const;

    // 展开参数模板，将参数转换为字符串，并存入 args_vec 中
    // 仅由 parseFormatString() 调用
    template <typename Type, typename... Args>
    void expandArgs(std::vector<std::string>& result, Type&& arg, Args&&... more_args);

    // 解析格式化字符串，将结果储存至 m_format_tokens 中
    template <typename... Args>
    void parseFormatString(const std::string& format_string, Args&&... args);

  private:
    void getCurrentTime(); // 获取当前时间，并存入 m_time 中
    void getThreadId();    // 获取当前线程ID，并存入 m_thread_id 中
};

#ifndef MYLOGGER_FORMATTER_INL_HPP
#include "formatter-inl.hpp"
MYLOGGER_FORMATTER_INL_HPP
#endif // MYLOGGER_FORMATTER_INL_HPP

#endif // MYLOGGER_FORMATTER_HPP
