// logger 类的具体实现

#pragma once

#ifndef MYLOGGER_LOGGER_INL_HPP
#define MYLOGGER_LOGGER_INL_HPP

#ifndef MYLOGGER_LOGGER_HPP
#include "logger.hpp"
#endif // MYLOGGER_LOGGER_HPP

#include "logwriter.hpp"
#include "threadspool.hpp"

inline Logger::Logger()
    : m_level(LogLevel::INFO), m_file_name("app.log"), m_console_output_enabled(true), m_file_output_enabled(false) {
}

inline Logger& Logger::getLogger() {
    static Logger instance;
    return instance;
}

inline void Logger::setLevel(LogLevel level) {
    static Logger& logger = getLogger();
    logger.m_level = level;
}

inline void Logger::enableConsole(bool enabled) {
    static Logger& logger = getLogger();
    logger.m_console_output_enabled = enabled;
}

inline void Logger::enabledFile(bool enabled) {
    static Logger& logger = getLogger();
    logger.m_file_output_enabled = enabled;
}

inline void Logger::setFile(const std::string& file_name) {
    static Logger& logger = getLogger();
    logger.m_file_name = file_name;
}

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

#endif // MYLOGGER_LOGGER_INL_HPP
