#ifndef ASSEMBLER_FIRST_PASS_H
#define ASSEMBLER_FIRST_PASS_H
#include "helpfunctions.h"
#include "errors.h"

void first_pass(char * file);
int read_line(FILE* file, line* sentence);
short int parse (line* sentence, error* error_list);
int find_semicolon(line* sentence);
void comment_check(line* sentence, error* error_list);
#endif /*ASSEMBLER_FIRST_PASS_H*/
