cmake_minimum_required(VERSION 4.1)
project(compilerStart)

set(CMAKE_CXX_STANDARD 20)

add_executable(compilerStart
        src/main.cpp
        src/LexAnalyzer.cpp
)
