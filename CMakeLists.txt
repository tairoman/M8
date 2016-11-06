
cmake_minimum_required(VERSION 3.3)
project(M8)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -fshort-enums")

set(SOURCE_FILES
        src/main.c
        src/vm/M8_vm.c
        src/vm/M8_vm.h
        src/vm/M8_helper.c
        src/vm/M8_helper.h
        src/vm/M8_instructions.h)

add_executable(M8 ${SOURCE_FILES})
