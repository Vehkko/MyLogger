
# MyLogger 🚀

MyLogger is a lightweight header-only C++ logging library that supports multi-level logging and provides flexible configuration for console and file output.

## 📂 Directory Structure

```
MyLogger
├── example
│   ├── CMakeLists.txt
│   └── main.cpp
├── include
│   └── MyLogger
│       ├── formatter.hpp
│       ├── level.hpp
│       ├── logger.hpp
│       ├── logwriter.hpp
│       └── threadspool.hpp
├── LICENSE
├── README.md
├── README_zh.md
└── src
    └── MyLogger
        ├── formatter.cpp
        ├── logger.cpp
        ├── logwriter.cpp
        └── threadspool.cpp
```

## 🔍 Project Background

I recently studied C++ template programming and multithreading. To practice these concepts, I developed this logging library project.

⚠️ This project is currently quite simple, with limited functionality and potential issues. Use with caution.

## ✨ Features
- 📌 Supports `DEBUG`, `INFO`, `WARNING`, and `ERROR` log levels
- 📁 Configurable output to console, file, or both
- 🔒 Thread-safe logging
- 📝 Supports formatted logs with automatic timestamps, thread IDs, log levels, etc.
- 🎭 Supports `{}` placeholders for formatting, compatible with `std::format` syntax
- 🚀 Simple and easy-to-use API

## 📦 Installation

Simply include the `include/MyLogger` directory in your project.

## 📥 Integrating into a Project

If your project uses CMake, you can integrate MyLogger by adding it as a subdirectory:

```cmake
add_subdirectory(MyLogger)
target_link_libraries(your_project PRIVATE MyLogger)
```

Alternatively, you can manually include the `MyLogger` header files in your project.

## 🛠 Usage Example

```cpp
#include "MyLogger/logger.hpp"

using log = Logger;

int main() {
    log::setLevel(LogLevel::DEBUG); // 默认日志等级为 INFO
    log::enableConsole(true);       // 开启控制台输出, 默认开启
    log::enabledFile(true);         // 关闭文件输出, 默认关闭
    log::setFile("app.log");        // 设置日志文件名为 app.log, 默认为 app.log

    log::debug("This is a debug message\n");               // 输出一条 DEBUG 级别日志
    log::info("This is an info message\n");                // 输出一条 INFO 级别日志
    log::warning("This is a warning message\n");           // 输出一条 WARNING 级别日志
    log::error("This is an error message\n");              // 输出一条 ERROR 级别日志
    log::log(LogLevel::INFO, "This is an info message\n"); // 可以通过 log() 函数输出日志，效果和前面的完全一致

    log::debugc("This is a debug message\n");               // 输出一条 DEBUG 级别日志, 仅输出到控制台.
    log::infoc("This is an info message\n");                // 输出一条 INFO 级别日志, 仅输出到控制台.
    log::warningc("This is a warning message\n");           // 输出一条 WARNING 级别日志, 仅输出到控制台.
    log::errorc("This is an error message\n");              // 输出一条 ERROR 级别日志, 仅输出到控制台.
    log::logc(LogLevel::INFO, "This is an info message\n"); // 可以通过 logc() 函数输出日志, 仅输出到控制台.

    log::debugf("This is a debug message\n");               // 输出一条 DEBUG 级别日志, 仅输出到文件.
    log::infof("This is an info message\n");                // 输出一条 INFO 级别日志, 仅输出到文件.
    log::warningf("This is a warning message\n");           // 输出一条 WARNING 级别日志, 仅输出到文件.
    log::errorf("This is an error message\n");              // 输出一条 ERROR 级别日志, 仅输出到文件.
    log::logf(LogLevel::INFO, "This is an info message\n"); // 可以通过 logf() 函数输出日志, 仅输出到文件.

    log::info("This is an {level} message\n"); // {level} 作为占位符, 会被替换为实际的日志等级, 这里会被替换为 INFO

    log::info("{time} This is an {level} message\n"); // {time} 作为占位符, 会被替换为当前时间, 这里会输出当前时间
    log::info("{time:%Y-%m-%d %H:%M:%S}, This is an {level} message\n"); // 当前时间的输出格式可以自定义, 这是默认的格式

    log::info("{thread} This is an {level} message\n"); // {thread} 作为占位符, 会被替换为当前线程 ID, 这里会输出当前线程 ID
    // 可以将线程 ID 输出为其他进制字符串, 默认十进制. 其中 x 为十六进制, o 为八进制, b 为二进制, d 为十进制. 大小写均可.
    log::info("{thread:x} This is an {level} message\n");

    int age = 18;
    std::string name = "Alice";
    // 后面的参数会自动匹配空占位符 "{}" 并将其替换, 这里会输出 "My name is Alice, and I'm 18 years old."
    log::info("My name is {}, and I'm {} years old.\n", name, age);
    // 也可以通过位置参数指定参数顺序. 位置参数必须是 unsigned int 类型, 不要求从 0 开始, 不要求连续, 会按照升序匹配.
    log::info("My name is {1}, and I'm {0} years old.\n", name, age);
    // 相同的参数可以多次使用, 会匹配到相同的值.
    log::info("I am {0}. I am {0}.\n", name);
    // 优先匹配指定位置参数的占位符, 再按顺序匹配空占位符. 剩余参数会直接拼接在后面. 若参数数量不够, 则会报错.
    log::info("My name is {1}, and I am {} years old.", name, age, " Nice to meet you.\n");

    return 0;
}
```

## 📊 Log Levels

MyLogger supports the following log levels:

- 🐛 `DEBUG`   - Debug information
- ℹ️  `INFO`    - General information
- ⚠️  `WARNING` - Warning messages
- ❌ `ERROR`   - Error messages

The default log level is `INFO`, which can be adjusted using `setLevel()`.

## 🔧 API Reference

- `Logger::setLevel(LogLevel level)`: Set the log level
- `Logger::enableConsole(bool enable)`: Enable/disable console output
- `Logger::enableFile(bool enable)`: Enable/disable file output
- `Logger::setFile(const std::string& filename)`: Set log file name
- `Logger::debug(const std::string& msg)`: Log a `DEBUG` level message
- `Logger::info(const std::string& msg)`: Log an `INFO` level message
- `Logger::warning(const std::string& msg)`: Log a `WARNING` level message
- `Logger::error(const std::string& msg)`: Log an `ERROR` level message
- `Logger::log(LogLevel level, const std::string& msg)`: Log a message at a specified level
- `Logger::debugc(const std::string& msg)`: Log a `DEBUG` level message to console only
- `Logger::infoc(const std::string& msg)`: Log an `INFO` level message to console only
- `Logger::warningc(const std::string& msg)`: Log a `WARNING` level message to console only
- `Logger::errorc(const std::string& msg)`: Log an `ERROR` level message to console only
- `Logger::logc(LogLevel level, const std::string& msg)`: Log a message at a specified level to console only
- `Logger::debugf(const std::string& msg)`: Log a `DEBUG` level message to file only
- `Logger::infof(const std::string& msg)`: Log an `INFO` level message to file only
- `Logger::warningf(const std::string& msg)`: Log a `WARNING` level message to file only
- `Logger::info(const std::string& format, const Args&... args)`: Formatted logging with `{}` placeholders and automatic metadata

## 📜 License

This project is licensed under the MIT License. See the LICENSE file for details.


