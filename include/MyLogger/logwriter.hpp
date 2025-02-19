// 对日志的输出进行简单封装

#pragma once

#ifndef MYLOGGER_LOGWRITER_HPP
#define MYLOGGER_LOGWRITER_HPP

#include <mutex>
#include <string>

static std::mutex m_console_mtx;
static std::mutex m_file_mtx;

class LogWriter {
  private:
    friend class Logger;

  private:
    LogWriter() = default;
    ~LogWriter() = default;
    LogWriter(const LogWriter&) = delete;
    LogWriter& operator=(const LogWriter&) = delete;
    // static LogWriter& getLogWriter();

  private:
    // 删除转义字符
    static std::string removeEscapeChar(const std::string& message);

  private:
    static void writeToConsole(const std::string& message);
    static void writeToFile(const std::string& filePath, const std::string& message);
};

#ifndef MYLOGGER_LOGWRITER_INL_HPP
#include "logwriter-inl.hpp"
MYLOGGER_LOGWRITER_INL_HPP
#endif

#endif // MYLOGGER_LOGWRITER_HPP
