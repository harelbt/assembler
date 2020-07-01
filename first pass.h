#ifndef ASSEMBLER_FIRST_PASS_H
#define ASSEMBLER_FIRST_PASS_H
#include "helpfunctions.h"
int first_pass(char * file);
int read_line(FILE* file);
void error_handle(char* line);
#endif //ASSEMBLER_FIRST_PASS_H
