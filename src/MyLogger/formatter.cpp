#include <bitset>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>

#include "MyLogger/formatter.hpp"

Formatter::Formatter(LogLevel level) : m_level(level) {
    getCurrentTime();
    getThreadId();
}

std::pair<Formatter::Token, std::string> Formatter::tokenize(const std::string& token_string) {
    std::pair<Token, std::string> result;

    auto ptr = token_string.find(':', 1);
    bool has_arg = true;
    if (ptr == std::string::npos) {
        ptr = token_string.size() - 1;
        has_arg = false;
    }

    std::string token_name = token_string.substr(1, ptr - 1);

    // 解析 "{level}"
    if (token_name == "level") {
        result.first = Token::LEVEL;
        switch (m_level) {
        case LogLevel::DEBUG:
            result.second = "DEBUG";
            break;
        case LogLevel::INFO:
            result.second = "INFO";
            break;
        case LogLevel::WARNING:
            result.second = "WARNING";
            break;
        case LogLevel::ERROR:
            result.second = "ERROR";
            break;
        }
    }

    // 解析 "{time:format}"
    else if (token_name == "time") {
        result.first = Token::TIME;
        auto now_time = std::chrono::system_clock::to_time_t(m_time);
        struct tm buf;
        localtime_r(&now_time, &buf);

        if (has_arg) {
            std::stringstream ss;
            ss << std::put_time(&buf, token_string.substr(ptr + 1, token_string.size() - ptr - 2).c_str());

            result.second = ss.str();
        } else {
            std::stringstream ss;
            ss << std::put_time(&buf, "%Y-%m-%d %H:%M:%S");
            result.second = ss.str();
        }
    }

    // 解析 "{thread:format}"
    else if (token_name == "thread") {
        result.first = Token::THREAD;
        if (has_arg) {
            std::string thread_arg = token_string.substr(ptr + 1, token_string.size() - ptr - 2);
            if (thread_arg == "B" || thread_arg == "b") {
                // 将 m_thread_id 转换为二进制数字字符串
                long long decimal = std::stoll(m_thread_id); // 将 m_thread_id 转为十进制整数
                std::bitset<64> binary(decimal);             // 假设最多 64 位
                result.second = binary.to_string();
                size_t pos = result.second.find('1');
                if (pos == std::string::npos) {
                    result.second = "0b0";
                }
                result.second = std::string("0b") + result.second.substr(pos); // 去除前导零
            } else if (thread_arg == "O" || thread_arg == "o") {
                // 将 m_thread_id 转换为八进制数字字符串
                long long decimal = std::stoll(m_thread_id);
                std::ostringstream oss;
                oss << "0o" << std::oct << decimal;
                result.second = oss.str();
            } else if (thread_arg == "D" || thread_arg == "d") {
                // 将 m_thread_id 转换为十进制数字字符串
                result.second = m_thread_id;
            } else if (thread_arg == "X" || thread_arg == "x") {
                // 将 m_thread_id 转换为十六进制数字字符串
                long long decimal = std::stoll(m_thread_id);
                std::ostringstream oss;
                oss << "0x" << std::hex << std::uppercase << decimal; // 输出大写字母
                result.second = oss.str();
            } else {
                throw std::runtime_error("Invalid format argument for thread token: " + thread_arg + ".");
            }
        } else {
            result.second = m_thread_id;
        }
    }

    // 不合法的 token
    else {
        throw std::runtime_error("Invalid format token: " + token_string + ".");
    }

    return result;
}

std::string Formatter::formatedString() const {
    std::string result;
    for (const auto& token : m_format_tokens) {
        result += token.second;
    }
    return result;
}

void Formatter::getCurrentTime() {
    m_time = std::chrono::system_clock::now();
}

void Formatter::getThreadId() {
    std::ostringstream oss;
#ifdef _WIN32
    oss << std::hex << std::this_thread::get_id().hash(); // Windows需特殊处理
#else
    oss << std::this_thread::get_id();
#endif
    m_thread_id = oss.str();
}
