#ifndef ASSEMBLER_FIRST_PASS_H
#define ASSEMBLER_FIRST_PASS_H
#include "helpfunctions.h"
#include "errors.h"

void first_pass(char * file);
int read_line(FILE* file, line* sentence);
void parse_line (line* sentence, error* error_list);
void comment_check(line* sentence, error* error_list);
void empty_line_check (line* sentence);
void find_signs(line* sentence, line_marks_index* indexes, error* error_list);
#endif /*ASSEMBLER_FIRST_PASS_H*/
