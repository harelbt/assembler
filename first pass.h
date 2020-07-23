#ifndef ASSEMBLER_FIRST_PASS_H
#define ASSEMBLER_FIRST_PASS_H
#include "helpfunctions.h"
void first_pass(char * file, short int* error_found);
static void print_errors_summary(char* file_name, int errors_count);
static void free_first_pass(FILE* filep, line * sentence);
static void free_line(line* sentence);
#endif /*ASSEMBLER_FIRST_PASS_H*/
