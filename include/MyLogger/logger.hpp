// 整个项目的核心文件，定义了Logger类，以及相关的函数接口，用户可以直接调用。

#pragma once

#include <string>

#include "MyLogger/formatter.hpp"
#include "MyLogger/logwriter.hpp"
#include "level.hpp"
#include "threadspool.hpp"

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

template <typename... Args>
void Logger::debug(const std::string& message, const Args&... args) {
    Logger& logger = getLogger();

    if (logger.m_level > LogLevel::DEBUG)
        return;

    if (!(logger.m_console_output_enabled || logger.m_file_output_enabled))
        return;

    bool console_output_enabled = logger.m_console_output_enabled;
    bool file_output_enabled = logger.m_file_output_enabled;
    std::string file_name = logger.m_file_name;

    // 由于 Formatter 在实例化的时候会获取线程id，所以这里不能在线程池中实例化，只能在主线程中实例化，然后在线程池中传入参数
    // 没必要用智能指针啦，就只在线程池中用一次，用完马上就 delete 掉啦
    Formatter* formatter(new Formatter(LogLevel::DEBUG));
    if (!formatter) {
        throw std::bad_alloc();
    }

    ThreadsPool::getThreadsPool().addFormatTask(
        [=](const Args... args) {
            formatter->parseFormatString(message, args...);
            std::string formated_string = formatter->formatedString();
            delete formatter;

            if (console_output_enabled) {
                ThreadsPool::getThreadsPool().addConsoleOutputTask(LogWriter::writeToConsole, formated_string);
            }

            if (file_output_enabled) {
                ThreadsPool::getThreadsPool().addFileOutputTask(LogWriter::writeToFile, file_name, formated_string);
            }
        },
        args...);
};

template <typename... Args>
void Logger::info(const std::string& message, const Args&... args) {
    Logger& logger = getLogger();

    if (logger.m_level > LogLevel::INFO)
        return;

    if (!(logger.m_console_output_enabled || logger.m_file_output_enabled))
        return;

    bool console_output_enabled = logger.m_console_output_enabled;
    bool file_output_enabled = logger.m_file_output_enabled;
    std::string file_name = logger.m_file_name;

    Formatter* formatter(new Formatter(LogLevel::INFO));
    if (!formatter) {
        throw std::bad_alloc();
    }

    ThreadsPool::getThreadsPool().addFormatTask(
        [=](const Args... args) {
            formatter->parseFormatString(message, args...);
            std::string formated_string = formatter->formatedString();
            delete formatter;

            if (console_output_enabled) {
                ThreadsPool::getThreadsPool().addConsoleOutputTask(LogWriter::writeToConsole, formated_string);
            }

            if (file_output_enabled) {
                ThreadsPool::getThreadsPool().addFileOutputTask(LogWriter::writeToFile, file_name, formated_string);
            }
        },
        args...);
}

template <typename... Args>
void Logger::warning(const std::string& message, const Args&... args) {
    Logger& logger = getLogger();

    if (logger.m_level > LogLevel::WARNING)
        return;

    if (!(logger.m_console_output_enabled || logger.m_file_output_enabled))
        return;

    bool console_output_enabled = logger.m_console_output_enabled;
    bool file_output_enabled = logger.m_file_output_enabled;
    std::string file_name = logger.m_file_name;

    Formatter* formatter(new Formatter(LogLevel::WARNING));
    if (!formatter) {
        throw std::bad_alloc();
    }

    ThreadsPool::getThreadsPool().addFormatTask(
        [=](const Args... args) {
            formatter->parseFormatString(message, args...);
            std::string formated_string = formatter->formatedString();
            delete formatter;

            if (console_output_enabled) {
                ThreadsPool::getThreadsPool().addConsoleOutputTask(LogWriter::writeToConsole, formated_string);
            }

            if (file_output_enabled) {
                ThreadsPool::getThreadsPool().addFileOutputTask(LogWriter::writeToFile, file_name, formated_string);
            }
        },
        args...);
}

template <typename... Args>
void Logger::error(const std::string& message, const Args&... args) {
    Logger& logger = getLogger();

    if (logger.m_level > LogLevel::ERROR)
        return;

    if (!(logger.m_console_output_enabled || logger.m_file_output_enabled))
        return;

    bool console_output_enabled = logger.m_console_output_enabled;
    bool file_output_enabled = logger.m_file_output_enabled;
    std::string file_name = logger.m_file_name;

    Formatter* formatter(new Formatter(LogLevel::ERROR));
    if (!formatter) {
        throw std::bad_alloc();
    }

    ThreadsPool::getThreadsPool().addFormatTask(
        [=](const Args... args) {
            formatter->parseFormatString(message, args...);
            std::string formated_string = formatter->formatedString();
            delete formatter;

            if (console_output_enabled) {
                ThreadsPool::getThreadsPool().addConsoleOutputTask(LogWriter::writeToConsole, formated_string);
            }

            if (file_output_enabled) {
                ThreadsPool::getThreadsPool().addFileOutputTask(LogWriter::writeToFile, file_name, formated_string);
            }
        },
        args...);
}

template <typename... Args>
void Logger::log(LogLevel level, const std::string& message, const Args&... args) {
    switch (level) {
    case LogLevel::DEBUG:
        debug(message, args...);
        break;
    case LogLevel::INFO:
        info(message, args...);
        break;
    case LogLevel::WARNING:
        warning(message, args...);
        break;
    case LogLevel::ERROR:
        error(message, args...);
        break;
    default:
        break;
    }
}

template <typename... Args>
void Logger::debugc(const std::string& message, const Args&... args) {
    Logger& logger = getLogger();

    if (logger.m_level > LogLevel::DEBUG)
        return;

    if (!(logger.m_console_output_enabled))
        return;

    Formatter* formatter(new Formatter(LogLevel::DEBUG));
    if (!formatter) {
        throw std::bad_alloc();
    }

    ThreadsPool::getThreadsPool().addFormatTask(
        [=](const Args... args) {
            formatter->parseFormatString(message, args...);
            std::string formated_string = formatter->formatedString();
            delete formatter;

            ThreadsPool::getThreadsPool().addConsoleOutputTask(LogWriter::writeToConsole, formated_string);
        },
        args...);
}

template <typename... Args>
void Logger::infoc(const std::string& message, const Args&... args) {
    Logger& logger = getLogger();

    if (logger.m_level > LogLevel::INFO)
        return;

    if (!(logger.m_console_output_enabled))
        return;

    Formatter* formatter(new Formatter(LogLevel::INFO));
    if (!formatter) {
        throw std::bad_alloc();
    }

    ThreadsPool::getThreadsPool().addFormatTask(
        [=](const Args... args) {
            formatter->parseFormatString(message, args...);
            std::string formated_string = formatter->formatedString();
            delete formatter;

            ThreadsPool::getThreadsPool().addConsoleOutputTask(LogWriter::writeToConsole, formated_string);
        },
        args...);
}

template <typename... Args>
void Logger::warningc(const std::string& message, const Args&... args) {
    Logger& logger = getLogger();

    if (logger.m_level > LogLevel::WARNING)
        return;

    if (!(logger.m_console_output_enabled))
        return;

    Formatter* formatter(new Formatter(LogLevel::WARNING));
    if (!formatter) {
        throw std::bad_alloc();
    }

    ThreadsPool::getThreadsPool().addFormatTask(
        [=](const Args... args) {
            formatter->parseFormatString(message, args...);
            std::string formated_string = formatter->formatedString();
            delete formatter;

            ThreadsPool::getThreadsPool().addConsoleOutputTask(LogWriter::writeToConsole, formated_string);
        },
        args...);
}

template <typename... Args>
void Logger::errorc(const std::string& message, const Args&... args) {
    Logger& logger = getLogger();

    if (logger.m_level > LogLevel::ERROR)
        return;

    if (!(logger.m_console_output_enabled))
        return;

    Formatter* formatter(new Formatter(LogLevel::ERROR));
    if (!formatter) {
        throw std::bad_alloc();
    }

    ThreadsPool::getThreadsPool().addFormatTask(
        [=](const Args... args) {
            formatter->parseFormatString(message, args...);
            std::string formated_string = formatter->formatedString();
            delete formatter;

            ThreadsPool::getThreadsPool().addConsoleOutputTask(LogWriter::writeToConsole, formated_string);
        },
        args...);
}

template <typename... Args>
void Logger::logc(LogLevel level, const std::string& message, const Args&... args) {
    switch (level) {
    case LogLevel::DEBUG:
        debugc(message, args...);
        break;
    case LogLevel::INFO:
        infoc(message, args...);
        break;
    case LogLevel::WARNING:
        warningc(message, args...);
        break;
    case LogLevel::ERROR:
        errorc(message, args...);
        break;
    default:
        break;
    }
}

template <typename... Args>
void Logger::debugf(const std::string& message, const Args&... args) {
    Logger& logger = getLogger();

    if (logger.m_level > LogLevel::DEBUG)
        return;

    if (!(logger.m_file_output_enabled))
        return;

    std::string file_name = logger.m_file_name;

    Formatter* formatter(new Formatter(LogLevel::DEBUG));
    if (!formatter) {
        throw std::bad_alloc();
    }

    ThreadsPool::getThreadsPool().addFormatTask(
        [=](const Args... args) {
            formatter->parseFormatString(message, args...);
            std::string formated_string = formatter->formatedString();
            delete formatter;

            ThreadsPool::getThreadsPool().addFileOutputTask(LogWriter::writeToFile, file_name, formated_string);
        },
        args...);
};

template <typename... Args>
void Logger::infof(const std::string& message, const Args&... args) {
    Logger& logger = getLogger();

    if (logger.m_level > LogLevel::INFO)
        return;

    if (!(logger.m_file_output_enabled))
        return;

    std::string file_name = logger.m_file_name;

    Formatter* formatter(new Formatter(LogLevel::INFO));
    if (!formatter) {
        throw std::bad_alloc();
    }

    ThreadsPool::getThreadsPool().addFormatTask(
        [=](const Args... args) {
            formatter->parseFormatString(message, args...);
            std::string formated_string = formatter->formatedString();
            delete formatter;

            ThreadsPool::getThreadsPool().addFileOutputTask(LogWriter::writeToFile, file_name, formated_string);
        },
        args...);
};

template <typename... Args>
void Logger::warningf(const std::string& message, const Args&... args) {
    Logger& logger = getLogger();

    if (logger.m_level > LogLevel::WARNING)
        return;

    if (!(logger.m_file_output_enabled))
        return;

    std::string file_name = logger.m_file_name;

    Formatter* formatter(new Formatter(LogLevel::WARNING));
    if (!formatter) {
        throw std::bad_alloc();
    }

    ThreadsPool::getThreadsPool().addFormatTask(
        [=](const Args... args) {
            formatter->parseFormatString(message, args...);
            std::string formated_string = formatter->formatedString();
            delete formatter;

            ThreadsPool::getThreadsPool().addFileOutputTask(LogWriter::writeToFile, file_name, formated_string);
        },
        args...);
};

template <typename... Args>
void Logger::errorf(const std::string& message, const Args&... args) {
    Logger& logger = getLogger();

    if (logger.m_level > LogLevel::ERROR)
        return;

    if (!(logger.m_file_output_enabled))
        return;

    std::string file_name = logger.m_file_name;

    Formatter* formatter(new Formatter(LogLevel::ERROR));
    if (!formatter) {
        throw std::bad_alloc();
    }

    ThreadsPool::getThreadsPool().addFormatTask(
        [=](const Args... args) {
            formatter->parseFormatString(message, args...);
            std::string formated_string = formatter->formatedString();
            delete formatter;

            ThreadsPool::getThreadsPool().addFileOutputTask(LogWriter::writeToFile, file_name, formated_string);
        },
        args...);
};

template <typename... Args>
void Logger::logf(LogLevel level, const std::string& message, const Args&... args) {
    switch (level) {
    case LogLevel::DEBUG:
        debugf(message, args...);
        break;
    case LogLevel::INFO:
        infof(message, args...);
        break;
    case LogLevel::WARNING:
        warningf(message, args...);
        break;
    case LogLevel::ERROR:
        errorf(message, args...);
        break;
    default:
        break;
    }
}
