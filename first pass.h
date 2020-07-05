#ifndef ASSEMBLER_FIRST_PASS_H
#define ASSEMBLER_FIRST_PASS_H
#include "helpfunctions.h"
#include "errors.h"

#define REGISTER_CONDITION ((i+1) < str_length && sentence->line[i+1] >= '0' && sentence->line[i+1] <= '9') &&\
((i > 0 && (sentence->line[i-1] == ' ' || sentence->line[i-1] == '\t' ))\
||(i == 0))
#define COMMENT_CONDITION indexes.semicolon_index == indexes.first_char_index && indexes.semicolon_index >= 0
#define UNEXPECTED_SEMICOLON_CONDITION indexes.semicolon_index > indexes.first_char_index &&\
indexes.semicolon_index > 0 && indexes.first_char_index >= 0
void first_pass(char * file);
int read_line(FILE* file, line* sentence);
void parse_line (line* sentence, error* error_list);
void comment_check(line* sentence, error* error_list, line_marks_index indexes);
void empty_line_check (line* sentence, line_marks_index indexes);
void find_signs(line* sentence, line_marks_index* indexes, error* error_list);
#endif /*ASSEMBLER_FIRST_PASS_H*/
