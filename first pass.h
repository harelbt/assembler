#ifndef ASSEMBLER_FIRST_PASS_H
#define ASSEMBLER_FIRST_PASS_H
#include "helpfunctions.h"

#define UNEXPECTED_SEMICOLON 1
void first_pass(char * file);
int read_line(FILE* file, line* sentence);
short int parse (line* sentence);
short int report_error(line* sentence, short int error_code);
int find_semicolon(line* sentence);
void comment_check(line* sentence);
#endif /*ASSEMBLER_FIRST_PASS_H*/
