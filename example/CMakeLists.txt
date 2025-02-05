cmake_minimum_required(VERSION 3.10)
project(logger)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -ggdb")
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

set(SRC_LIST ./main.cpp ../src/MyLogger/logger.cpp ../src/MyLogger/threadspool.cpp ../src/MyLogger/formatter.cpp ../src/MyLogger/logwriter.cpp)

set(INCLUDE_PATH ../include)

include_directories(${INCLUDE_PATH})

add_executable(${PROJECT_NAME} ${SRC_LIST})
