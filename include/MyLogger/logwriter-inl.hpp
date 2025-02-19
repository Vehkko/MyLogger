// logwriter 类的具体实现

#pragma once

#ifndef MYLOGGER_LOGWRITER_INL_HPP
#define MYLOGGER_LOGWRITER_INL_HPP

#ifndef MYLOGGER_LOGWRITER_HPP
#include "logwriter.hpp"
#endif // MYLOGGER_LOGWRITER_HPP

#include <fstream>
#include <iostream>

// inline LogWriter& LogWriter::getLogWriter() {
//     static LogWriter logWritter;
//     return logWritter;
// }

inline std::string LogWriter::removeEscapeChar(const std::string& message) {
    std::string result;
    result.reserve(message.size());
    std::string::size_type pos_left = 0;
    std::string::size_type pos_right = 0;
    while (pos_right < message.size()) {
        while (pos_right < message.size()) {
            if (message[pos_right] == -128) {
                result.append(message.substr(pos_left, pos_right - pos_left));
                pos_right++;
                pos_left = pos_right;
                break;
            }
            pos_right++;
        }
    }
    if (pos_left != pos_right)
        result.append(message.substr(pos_left));
    return result;
}

inline void LogWriter::writeToConsole(const std::string& message) {
    std::unique_lock<std::mutex> lock(m_console_mtx);
    std::cout << removeEscapeChar(message);
}

inline void LogWriter::writeToFile(const std::string& filePath, const std::string& message) {
    std::unique_lock<std::mutex> lock(m_file_mtx);
    std::ofstream file(filePath, std::ios::app);
    if (file.is_open()) {
        file << removeEscapeChar(message);
        file.close();
    }
}

#endif // MYLOGGER_LOGWRITER_INL_HPP
