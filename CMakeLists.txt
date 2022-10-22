
cmake_minimum_required(VERSION 3.23)
project(FChip8 C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(FChip8
        src/main.c
        src/chip8.c
        src/chip8disasm.c
        )
