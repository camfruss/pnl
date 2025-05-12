
cmake_minimum_required(VERSION 3.29)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project_name(pnl_calculator)

add_executable(
    pnl_calculator
    main.cpp
)
