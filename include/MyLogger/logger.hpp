// 整个项目的核心文件，定义了Logger类，以及相关的函数接口，用户可以直接调用。

#pragma once

#ifndef MYLOGGER_LOGGER_HPP
#define MYLOGGER_LOGGER_HPP

#include <string>

#include "formatter.hpp"
#include "loglevel.hpp"

class Logger {
  private:
    LogLevel m_level;
    std::string m_file_name;
    bool m_console_output_enabled;
    bool m_file_output_enabled;

  private:
    Logger();
    ~Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static Logger& getLogger();

  public:
    template <typename... Args>
    static void debug(const std::string& message, const Args&... args);

    template <typename... Args>
    static void info(const std::string& message, const Args&... args);

    template <typename... Args>
    static void warning(const std::string& message, const Args&... args);

    template <typename... Args>
    static void error(const std::string& message, const Args&... args);

    template <typename... Args>
    static void log(LogLevel level, const std::string& message, const Args&... args);

    template <typename... Args>
    static void debugc(const std::string& message, const Args&... args);

    template <typename... Args>
    static void infoc(const std::string& message, const Args&... args);

    template <typename... Args>
    static void warningc(const std::string& message, const Args&... args);

    template <typename... Args>
    static void errorc(const std::string& message, const Args&... args);

    template <typename... Args>
    static void logc(LogLevel level, const std::string& message, const Args&... args);

    template <typename... Args>
    static void debugf(const std::string& message, const Args&... args);

    template <typename... Args>
    static void infof(const std::string& message, const Args&... args);

    template <typename... Args>
    static void warningf(const std::string& message, const Args&... args);

    template <typename... Args>
    static void errorf(const std::string& message, const Args&... args);

    template <typename... Args>
    static void logf(LogLevel level, const std::string& message, const Args&... args);

  public:
    static void setLevel(LogLevel level);
    static void enableConsole(bool enabled);
    static void enabledFile(bool enabled);
    static void setFile(const std::string& file_name);
};

#ifndef MYLOGGER_LOGGER_INL_HPP
#include "logger-inl.hpp"
MYLOGGER_LOGGER_INL_HPP // 调用一下 logger-inl.hpp 里定义的宏, 避免编译器警告
#endif                  // MYLOGGER_LOGGER_INL_HPP

#endif // MYLOGGER_LOGGER_HPP
