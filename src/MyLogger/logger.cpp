#include "MyLogger/logger.hpp"

Logger::Logger()
    : m_level(LogLevel::INFO), m_file_name("app.log"), m_console_output_enabled(true), m_file_output_enabled(false) {
}

Logger& Logger::getLogger() {
    static Logger instance;
    return instance;
}

void Logger::setLevel(LogLevel level) {
    static Logger& logger = getLogger();
    logger.m_level = level;
}

void Logger::enableConsole(bool enabled) {
    static Logger& logger = getLogger();
    logger.m_console_output_enabled = enabled;
}

void Logger::enabledFile(bool enabled) {
    static Logger& logger = getLogger();
    logger.m_file_output_enabled = enabled;
}

void Logger::setFile(const std::string& file_name) {
    static Logger& logger = getLogger();
    logger.m_file_name = file_name;
}
