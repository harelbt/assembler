#ifndef ASSEMBLER_FIRST_PASS_H
#define ASSEMBLER_FIRST_PASS_H
#include "helpfunctions.h"


void first_pass(char * file);
int read_line(FILE* file, line* sentence);
short int parse (line* sentence);

int find_semicolon(line* sentence);
void comment_check(line* sentence);
#endif /*ASSEMBLER_FIRST_PASS_H*/
