cmake_minimum_required(VERSION 3.16)
project(assembler C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(assembler
        helpfunctions.c
        helpfunctions.h
        words.c
        words.h assembler.c "first pass.c" "first pass.h" "assembler data types.c" "assembler data types.h" errors.c errors.h "line analyzer.c" "line analyzer.h" "symbol table.c" "symbol table.h" translator.c translator.h assembler.h "data image.c" "data image.h" "second pass.c" "second pass.h")
