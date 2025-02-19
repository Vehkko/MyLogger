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
