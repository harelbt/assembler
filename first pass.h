#ifndef ASSEMBLER_FIRST_PASS_H
#define ASSEMBLER_FIRST_PASS_H
#include "helpfunctions.h"
void first_pass(char * file);
int read_line(FILE* file);
short int parse (char* label, char* operator, char* first_operand, char* second_operand);
short int error_check(char* line);
#endif /*ASSEMBLER_FIRST_PASS_H*/
