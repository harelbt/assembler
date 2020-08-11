
#define STRING_LEN 50
#define E 1
#define R 2
#define A 4
int second_pass(FILE * first_pass_file,symbol * symbol_table,FILE * input_file);
void resetString(char str[STRING_LEN]);
#ifndef ASSEMBLER_SECOND_PASS_H
#define ASSEMBLER_SECOND_PASS_H
#include <stdio.h>
#include "assembler data types.h"
static void code_symbols(FILE* machine_code, symbol* symbol_table);
static void add_entries();
static void prepare_output_files();
#endif //ASSEMBLER_SECOND_PASS_H
