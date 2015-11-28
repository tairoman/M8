
cmake_minimum_required(VERSION 3.3)
project(M8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c99 -fshort-enums")

set(SOURCE_FILES
    VM/M8_interpreter.c
    VM/M8_interpreter.h
    README.md)

add_executable(M8 ${SOURCE_FILES})