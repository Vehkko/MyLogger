// 对日志的输出进行简单封装

#pragma once

#include <mutex>
#include <string>

class LogWriter {
  private:
    friend class Logger;

  private:
    static std::mutex m_console_mtx;
    static std::mutex m_file_mtx;

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
