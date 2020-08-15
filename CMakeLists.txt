cmake_minimum_required(VERSION 3.15.3)
project(assembler C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(assembler
        second_pass.c assembler.c "first pass.c" "first pass.h" "assembler data types.c" "assembler data types.h" errors.c errors.h "line analyzer.c" "line analyzer.h" "symbol table.c" "symbol table.h" translator.c translator.h "in out tools.c" "in out tools.h")
