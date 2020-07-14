
#ifndef ASSEMBLER_ERRORS_H
#define ASSEMBLER_ERRORS_H
/*unexpected chars*/
#define UNEXPECTED_CHARACTER 0
#define UNEXPECTED_SEMICOLON 1
#define UNEXPECTED_HASHMARK 2
#define UNEXPECTED_REGISTER 3
#define UNEXPECTED_QUOT_MARK 4
/*CODE*/
#define REGISTER_NO_OPERATOR 5
#define HASHMARK_NO_OPERATOR 6
#define EXTRA_OPERATORS 7
/*sentence types*/
#define MIXED_SENTENCE 8
#define NO_SENTENCE_TYPE 9
/*STRINGS*/
#define SINGLE_QUOTMARK 10
#define STRING_NO_ORDER 11
#define CODE_AFTER_QUOTE 12
#define ORDER_NO_STRING 13
#define CHARS_BETWEEN_ORDER_STRING 14
#define CHARS_BETWEEN_LABEL_STRINGORDER 15
#define CHARS_BEFORE_STRING_ORDER 16
/*DATA*/
#define DATA_NO_ORDER 17
#define ORDER_NO_DATA 18
#define COMMA_NO_NUMBERS 19
#define NO_COMMA_BETWEEN 20
#define NO_NUMBERS_BETWEEN_COMMAS 21
#include "assembler data types.h"

short int errors_inspection(line sentence, line_marks_index indexes, line_marks_counter counters);
static void report_error(char* line, short int error_code, line_marks_counter counters, ...);
static short int inspect_order_line(line sentence, line_marks_index indexes, line_marks_counter counters);
static short int inspect_code_line(line sentence, line_marks_index indexes, line_marks_counter counters);
static short int detect_string_errors(line sentence, line_marks_index indexes, line_marks_counter counters);
static short int detect_data_errors(line sentence, line_marks_index indexes, line_marks_counter counters);
static short int is_data_values_proper(line sentence, line_marks_index indexes, line_marks_counter counters);
static short int inspect_data_values(char* line, int index, line_marks_counter counters);
#endif /*ASSEMBLER_ERRORS_H*/
