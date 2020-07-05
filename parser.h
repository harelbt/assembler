#ifndef ASSEMBLER_PARSER_H
#define ASSEMBLER_PARSER_H
#include "errors.h"

#define REGISTER_CONDITION ((i+1) < str_length && sentence->line[i+1] >= '0' && sentence->line[i+1] <= '9') &&\
((i > 0 && (sentence->line[i-1] == ' ' || sentence->line[i-1] == '\t' ))\
||(i == 0))
#define COMMENT_CONDITION indexes.semicolon_index == indexes.first_char_index && indexes.semicolon_index >= 0
#define UNEXPECTED_SEMICOLON_CONDITION indexes.semicolon_index > indexes.first_char_index &&\
indexes.semicolon_index > 0 && indexes.first_char_index >= 0

 void parse_line (line* sentence, error* error_list, char* operators_table);
static void comment_check(line* sentence, error* error_list, line_marks_index indexes);
static void empty_line_check (line* sentence, line_marks_index indexes);
static void find_signs(line* sentence, line_marks_index* indexes, error* error_list);
void extract_operator(line* sentence, char* operators_table, line_marks_index indexes, error* error_list);
static int recognize_operator(char* operator, char* operators_table,int* opcode, int* function);
static void find_and_handle_operators(int str_length, char* operator, line sentence, int* number_of_operators, char* operators_table);
static void handle_operators(int number_of_operators, line* sentence, error* error_list);
static void define_as_not_instruction(line* sentence);
static void assume_no_signes(line_marks_index* indexes);

#endif //ASSEMBLER_PARSER_H
