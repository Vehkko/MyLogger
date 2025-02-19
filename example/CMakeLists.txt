cmake_minimum_required(VERSION 3.10)
project(logger)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -ggdb -O0")
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

set(SRC_LIST ./main.cpp)

set(INCLUDE_PATH ../include)

include_directories(${INCLUDE_PATH})

add_executable(${PROJECT_NAME} ${SRC_LIST})
