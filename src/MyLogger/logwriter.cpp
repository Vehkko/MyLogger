#include <fstream>
#include <iostream>
#include <mutex>

#include "MyLogger/logwriter.hpp"

std::mutex LogWriter::m_console_mtx;
std::mutex LogWriter::m_file_mtx;

// LogWriter& LogWriter::getLogWriter() {
//     static LogWriter logWritter;
//     return logWritter;
// }

std::string LogWriter::removeEscapeChar(const std::string& message) {
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

void LogWriter::writeToConsole(const std::string& message) {
    std::unique_lock<std::mutex> lock(LogWriter::m_console_mtx);
    std::cout << removeEscapeChar(message) << '\n';
}

void LogWriter::writeToFile(const std::string& filePath, const std::string& message) {
    std::unique_lock<std::mutex> lock(LogWriter::m_file_mtx);
    std::ofstream file(filePath, std::ios::app);
    if (file.is_open()) {
        file << removeEscapeChar(message) << '\n';
        file.close();
    }
}
