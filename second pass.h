#ifndef ASSEMBLER_SECOND_PASS_H
#define ASSEMBLER_SECOND_PASS_H
#include <stdio.h>
#include "assembler data types.h"
static void code_symbols(FILE* machine_code, symbol* symbol_table);
static void add_entries();
static void prepare_output_files();
#endif //ASSEMBLER_SECOND_PASS_H
