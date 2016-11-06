
cmake_minimum_required(VERSION 3.3)
project(M8)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -fshort-enums")

set(SOURCE_FILES
    src/M8_vm.c)

add_executable(M8 ${SOURCE_FILES})
