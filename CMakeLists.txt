
cmake_minimum_required(VERSION 3.29)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(pnl_calculator
    VERSION 0.0.1
    LANGUAGES CXX
)

include_directories(include)

add_executable(
    pnl_calculator
    src/main.cpp
    src/pnl_calculator.cpp
)
