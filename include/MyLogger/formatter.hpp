// 格式化字符串解析器，所有成员均设置为私有，禁止用户直接访问，只能通过 Logger 类进行操作。

#pragma once

#include <chrono>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "level.hpp"

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

template <typename Type, typename... Args>
void Formatter::expandArgs(std::vector<std::string>& args_vec, Type&& arg, Args&&... more_args) {

    // 将参数转换为字符串，并存入 args_vec 中
    std::ostringstream oss;
    oss << arg;
    args_vec.push_back(oss.str());

    // 递归展开参数模板
    if constexpr (sizeof...(more_args) > 0) {
        expandArgs(args_vec, std::forward<Args>(more_args)...);
    }
}

template <typename... Args>
void Formatter::parseFormatString(const std::string& format_string_input, Args&&... args) {
    m_format_tokens.clear();
    m_format_tokens.reserve(10); // 预留空间，避免多次扩容

    std::string format_string = format_string_input; // 拷贝一份，以免修改输入参数

    std::vector<unsigned int> brackets; // 括号的位置

    bool in_brackets = false;
    for (unsigned int i = 0; i < format_string.size(); i++) {
        if (format_string[i] == '{') {
            if (i != 0 && format_string[i - 1] == '\%') {
                // 跳过转义的 %{
                format_string[i - 1] = -128;
                continue;
            }
            if (in_brackets) {
                throw std::runtime_error("Invalid format string: missing closing '}' character.");
            }
            in_brackets = true;
            brackets.push_back(i);
        } else if (format_string[i] == '}') {
            if (i != 0 && format_string[i - 1] == '\%') {
                // 跳过转义的 %}
                format_string[i - 1] = -128;
                continue;
            }
            if (!in_brackets) {
                throw std::runtime_error("Invalid format string: missing opening '{' character.");
            }
            in_brackets = false;
            brackets.push_back(i);
        }
    }

    std::vector<std::string> args_vec; // 用于存放展开的参数包
    args_vec.reserve(sizeof...(args));
    if constexpr (sizeof...(args) > 0) {
        expandArgs(args_vec, std::forward<Args>(args)...);
    }

    std::vector<unsigned int> empty_brackets_groups;                   // 空括号组在 m_format_tokens 中的位置
    std::map<unsigned int, std::vector<unsigned int>> brackets_groups; // 括号组({0}、{1}...)在 m_format_tokens 中的位置

    // 判断 brackets 是否为空，即字符串中是否有 {}
    if (brackets.empty()) {
        m_format_tokens.emplace_back(Token::TEXT, format_string);
        return;
    }

    in_brackets = true;

    // 处理第一段格式化字符串
    if (brackets[0] != 0) {
        m_format_tokens.emplace_back(Token::TEXT, format_string.substr(0, brackets[0]));
    }

    // 处理第一个 '{' 及以后的所有 tokens
    for (unsigned int i = 0; i < brackets.size() - 1; i++) {

        if (!in_brackets) {
            // 不在{}内：解析为文本(Token::TEXT)

            if (brackets[i + 1] - brackets[i] > 1) {
                // 不是空字符串
                m_format_tokens.emplace_back(Token::TEXT,
                                             format_string.substr(brackets[i] + 1, brackets[i + 1] - brackets[i] - 1));
                in_brackets = true;
                continue;
            } else {
                // 空字符串
                in_brackets = true;
                continue;
            }

        } else {
            // 在{}内：解析为其他 Token 类型

            if (brackets[i + 1] - brackets[i] != 1) {
                // 非空格式化字符串: 需要解析

                std::string token_string = format_string.substr(brackets[i], brackets[i + 1] - brackets[i] + 1);

                // 检查括号组里的字符串是不是纯数字，如 {0}、{1}...
                bool is_number = true;
                for (unsigned int j = 1; j < token_string.size() - 1; j++) {
                    if (!isdigit(token_string[j])) {
                        is_number = false;
                        break;
                    }
                }

                if (is_number) {
                    // 解析数字格式化字符串
                    m_format_tokens.emplace_back(Token::ARG, ""); // 先传入一个空字符串占位

                    unsigned int num = std::stoi(token_string.substr(1, token_string.size() - 2));

                    auto index = brackets_groups.find(num);
                    if (index != brackets_groups.end()) {
                        // 找到重复索引: 直接添加
                        brackets_groups[num].push_back(m_format_tokens.size() - 1);
                    } else {
                        // 未找到重复索引，插入新索引
                        std::vector<unsigned int> vector_temp;
                        vector_temp.push_back(m_format_tokens.size() - 1);
                        brackets_groups.insert(std::make_pair(num, std::move(vector_temp)));
                    }
                } else {
                    // 通过 tokenize() 解析单个格式化字符串，返回 Token 和内容
                    m_format_tokens.emplace_back(tokenize(std::move(token_string)));
                }

            } else {
                // 空格式化字符串：直接匹配参数

                m_format_tokens.emplace_back(Token::ARG, "");
                empty_brackets_groups.push_back(m_format_tokens.size() - 1);

                // if (args_count > sizeof...(args)) {
                //     throw std::runtime_error("Invalid format string: too many arguments provided.");
                // } else {
                //     m_format_tokens.emplace_back(Token::ARG, args_vec[args_count++]);
                // }
            }
            in_brackets = false;
            continue;
        }
    }

    // 处理最后一段格式化字符串
    if (brackets.back() != format_string.size() - 1) {
        m_format_tokens.emplace_back(Token::TEXT, format_string.substr(brackets.back() + 1));
    }

    unsigned int args_index = 0; // 用于记录参数索引

    // 注意，map 容器会自动将键值对按照 key 的升序排列，因此不需要再次排序
    // 处理数字括号组
    for (const auto& [key, value] : brackets_groups) {
        if (args_index >= sizeof...(args)) {
            throw std::runtime_error("Invalid format string: too few arguments provided.");
        }

        std::ignore = key;
        for (auto index : value) {
            m_format_tokens[index].second = args_vec[args_index];
        }
        args_index++;
    }
    // 处理空括号组
    for (const auto& index : empty_brackets_groups) {
        if (args_index >= sizeof...(args)) {
            throw std::runtime_error("Invalid format string: too few arguments provided.");
        }

        m_format_tokens[index].second = args_vec[args_index++];
    }

    // 将剩余的格式化字符串直接添加到末尾
    for (unsigned int i = args_index; i < sizeof...(args); i++) {
        m_format_tokens.emplace_back(Token::ARG, args_vec[i]);
    }
}
