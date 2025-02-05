
# MyLogger 🚀

MyLogger is a lightweight C++ logging library that supports multi-level logging and provides flexible configuration for console and file output.

## 📂 Directory Structure

```
MyLogger/
├── include/
│   ├── MyLogger/
│   │   ├── formatter.hpp
│   │   ├── level.hpp
│   │   ├── logger.hpp
│   │   ├── logwriter.hpp
│   │   ├── threadpool.hpp
├── src/
│   ├── formatter.cpp
│   ├── logger.cpp
│   ├── logwriter.cpp
│   ├── threadpool.cpp
├── examples/
│   ├── main.cpp
│   ├── CMakeLists.txt
├── README.md
├── README_zh.md
├── LICENSE
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

Include the `MyLogger` directory in your project and include the necessary header files in your compilation.

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

    log::setLevel(LogLevel::DEBUG); // Set the default log level to INFO
    log::enableConsole(true);       // Enable console output, enabled by default
    log::enabledFile(true);         // Disable file output, disabled by default
    log::setFile("app.log");        // Set the log file name to app.log, default is app.log

    log::debug("This is a debug message");               // Output a DEBUG level log
    log::info("This is an info message");                // Output an INFO level log
    log::warning("This is a warning message");           // Output a WARNING level log
    log::error("This is an error message");              // Output an ERROR level log
    log::log(LogLevel::INFO, "This is an info message"); // Log can also be output using the log() function, same as above

    log::debugc("This is a debug message");               // Output a DEBUG level log, only to the console.
    log::infoc("This is an info message");                // Output an INFO level log, only to the console.
    log::warningc("This is a warning message");           // Output a WARNING level log, only to the console.
    log::errorc("This is an error message");              // Output an ERROR level log, only to the console.
    log::logc(LogLevel::INFO, "This is an info message"); // Log can also be output using the logc() function, only to the console.

    log::debugf("This is a debug message");               // Output a DEBUG level log, only to the file.
    log::infof("This is an info message");                // Output an INFO level log, only to the file.
    log::warningf("This is a warning message");           // Output a WARNING level log, only to the file.
    log::errorf("This is an error message");              // Output an ERROR level log, only to the file.
    log::logf(LogLevel::INFO, "This is an info message"); // Log can also be output using the logf() function, only to the file.

    log::info("This is an {level} message"); // {level} as a placeholder, will be replaced by the actual log level, here it will be replaced with INFO

    log::info("{time} This is an {level} message"); // {time} as a placeholder, will be replaced by the current time, here it will output the current time
    log::info("{time:%Y-%m-%d %H:%M:%S}, This is an {level} message"); // The output format of the current time can be customized, this is the default format

    log::info("{thread} This is an {level} message"); // {thread} as a placeholder, will be replaced by the current thread ID, here it will output the current thread ID
    // The thread ID can be output as a string in other number systems, default is decimal. x for hexadecimal, o for octal, b for binary, d for decimal. Both lowercase and uppercase are valid.
    log::info("{thread:x} This is an {level} message");

    int age = 18;
    std::string name = "Alice";
    // The following parameters will automatically match empty placeholders "{}" and replace them, here it will output "My name is Alice, and I'm 18 years old."
    log::info("My name is {}, and I'm {} years old.", name, age);
    // You can also specify the order of parameters using position arguments. The position arguments must be of unsigned int type, they don't need to start from 0 or be consecutive, they will match in ascending order.
    log::info("My name is {1}, and I'm {0} years old.", name, age);
    // The same parameter can be used multiple times, and it will match the same value.
    log::info("I am {0}. I am {0}.", name);
    // The placeholders for the specified position parameters will be matched first, followed by empty placeholders. The remaining parameters will be directly concatenated. If there are not enough parameters, it will throw an error.
    log::info("My name is {1}, and I am {} years old.", name, age, " Nice to meet you.");

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


