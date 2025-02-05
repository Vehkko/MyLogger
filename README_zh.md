# MyLogger 🚀

MyLogger 是一个轻量级的 C++ 日志库，支持多级别日志记录，并提供控制台输出和文件输出的灵活配置。

## 📂 目录结构

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

## 🔍 项目背景

最近学习了 C++ 模板编程与多线程编程等相关内容，为了实践这些知识，编写了这个日志库项目。

⚠️ 该项目目前较为简陋，功能有限，可能存在潜在问题，请慎用。

## ✨ 特性
- 📌 支持 `DEBUG`、`INFO`、`WARNING`、`ERROR` 级别的日志记录
- 📁 可配置日志输出到控制台、文件或两者兼有
- 🔒 线程安全的日志写入
- 📝 支持格式化日志，可自动填充时间、线程 ID、日志等级等信息
- 🎭 支持占位符 `{}` 进行格式化，基本兼容 C++20 的 `std::format` 语法
- 🚀 简单易用的 API

## 📦 安装

将 `MyLogger` 目录包含到你的项目中，并在编译时包含相关头文件。

## 🛠 使用示例

```cpp
#include "MyLogger/logger.hpp"

using log = Logger;

int main() {

    log::setLevel(LogLevel::DEBUG); // 默认日志等级为 INFO
    log::enableConsole(true);       // 开启控制台输出, 默认开启
    log::enabledFile(true);         // 关闭文件输出, 默认关闭
    log::setFile("app.log");        // 设置日志文件名为 app.log, 默认为 app.log

    log::debug("This is a debug message");               // 输出一条 DEBUG 级别日志
    log::info("This is an info message");                // 输出一条 INFO 级别日志
    log::warning("This is a warning message");           // 输出一条 WARNING 级别日志
    log::error("This is an error message");              // 输出一条 ERROR 级别日志
    log::log(LogLevel::INFO, "This is an info message"); // 可以通过 log() 函数输出日志，效果和前面的完全一致

    log::debugc("This is a debug message");               // 输出一条 DEBUG 级别日志, 仅输出到控制台.
    log::infoc("This is an info message");                // 输出一条 INFO 级别日志, 仅输出到控制台.
    log::warningc("This is a warning message");           // 输出一条 WARNING 级别日志, 仅输出到控制台.
    log::errorc("This is an error message");              // 输出一条 ERROR 级别日志, 仅输出到控制台.
    log::logc(LogLevel::INFO, "This is an info message"); // 可以通过 logc() 函数输出日志, 仅输出到控制台.

    log::debugf("This is a debug message");               // 输出一条 DEBUG 级别日志, 仅输出到文件.
    log::infof("This is an info message");                // 输出一条 INFO 级别日志, 仅输出到文件.
    log::warningf("This is a warning message");           // 输出一条 WARNING 级别日志, 仅输出到文件.
    log::errorf("This is an error message");              // 输出一条 ERROR 级别日志, 仅输出到文件.
    log::logf(LogLevel::INFO, "This is an info message"); // 可以通过 logf() 函数输出日志, 仅输出到文件.

    log::info("This is an {level} message"); // {level} 作为占位符, 会被替换为实际的日志等级, 这里会被替换为 INFO

    log::info("{time} This is an {level} message"); // {time} 作为占位符, 会被替换为当前时间, 这里会输出当前时间
    log::info("{time:%Y-%m-%d %H:%M:%S}, This is an {level} message"); // 当前时间的输出格式可以自定义, 这是默认的格式

    log::info("{thread} This is an {level} message"); // {thread} 作为占位符, 会被替换为当前线程 ID, 这里会输出当前线程 ID
    // 可以将线程 ID 输出为其他进制字符串, 默认十进制. 其中 x 为十六进制, o 为八进制, b 为二进制, d 为十进制. 大小写均可.
    log::info("{thread:x} This is an {level} message");

    int age = 18;
    std::string name = "Alice";
    // 后面的参数会自动匹配空占位符 "{}" 并将其替换, 这里会输出 "My name is Alice, and I'm 18 years old."
    log::info("My name is {}, and I'm {} years old.", name, age);
    // 也可以通过位置参数指定参数顺序. 位置参数必须是 unsigned int 类型, 不要求从 0 开始, 不要求连续, 会按照升序匹配.
    log::info("My name is {1}, and I'm {0} years old.", name, age);
    // 相同的参数可以多次使用, 会匹配到相同的值.
    log::info("I am {0}. I am {0}.", name);
    // 优先匹配指定位置参数的占位符, 再按顺序匹配空占位符. 剩余参数会直接拼接在后面. 若参数数量不够, 则会报错.
    log::info("My name is {1}, and I am {} years old.", name, age, " Nice to meet you.");

    return 0;
}
```

## 📊 日志级别

MyLogger 支持以下日志级别：

- 🐛 `DEBUG`   - 调试信息
- ℹ️  `INFO`    - 一般信息
- ⚠️  `WARNING` - 警告信息
- ❌ `ERROR`   - 错误信息

日志默认级别为 `INFO`，可通过 `setLevel()` 进行调整。

## 🔧 API 说明

- `Logger::setLevel(LogLevel level)`: 设置日志级别
- `Logger::enableConsole(bool enable)`: 开启/关闭控制台输出
- `Logger::enableFile(bool enable)`: 开启/关闭文件输出
- `Logger::setFile(const std::string& filename)`: 设置日志文件名
- `Logger::debug(const std::string& msg)`: 记录 DEBUG 级别日志
- `Logger::info(const std::string& msg)`: 记录 INFO 级别日志
- `Logger::warning(const std::string& msg)`: 记录 WARNING 级别日志
- `Logger::error(const std::string& msg)`: 记录 ERROR 级别日志
- `Logger::log(LogLevel level, const std::string& msg)`: 记录指定级别日志
- `Logger::debugc(const std::string& msg)`: 仅输出 DEBUG 级别日志到控制台
- `Logger::infoc(const std::string& msg)`: 仅输出 INFO 级别日志到控制台
- `Logger::warningc(const std::string& msg)`: 仅输出 WARNING 级别日志到控制台
- `Logger::errorc(const std::string& msg)`: 仅输出 ERROR 级别日志到控制台
- `Logger::logc(LogLevel level, const std::string& msg)`: 仅输出指定级别日志到控制台
- `Logger::debugf(const std::string& msg)`: 仅输出 DEBUG 级别日志到文件
- `Logger::infof(const std::string& msg)`: 仅输出 INFO 级别日志到文件
- `Logger::warningf(const std::string& msg)`: 仅输出 WARNING 级别日志到文件
- `Logger::info(const std::string& format, const Args&... args)`: 格式化日志，支持占位符 `{}` 并自动填充时间、线程 ID、日志等级等信息

## 📜 许可证

本项目使用 MIT 许可证，详情请见 LICENSE 文件。


