#ifndef ASSEMBLER_FIRST_PASS_H
#define ASSEMBLER_FIRST_PASS_H
#include "helpfunctions.h"
/*FUNCTIONS DECLARATION*/
symbol* first_pass(char * file, char* error_found);
static void free_first_pass(FILE* filep, line * sentence);
static void free_line(line* sentence);
/**/
#endif /*ASSEMBLER_FIRST_PASS_H*/
