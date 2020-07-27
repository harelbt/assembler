
#ifndef ASSEMBLER_ERRORS_H
#define ASSEMBLER_ERRORS_H
#include "assembler data types.h"
/*modes*/
#define LABEL_DEFINITION 100
#define LABEL_USAGE 101
/*ERRORS*/
/*unexpected chars*/
#define UNEXPECTED_CHARACTER 0
/*CODE*/
#define UNEXPECTED_REGISTER 3
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
#define STRING_BEFORE_STRING_ORDER 23
#define NO_SPACE_BEFORE_QUOTE 35
/*DATA*/
#define DATA_NO_ORDER 17
#define ORDER_NO_DATA 18
#define COMMA_NO_NUMBERS 19
#define NO_COMMA_BETWEEN 20
#define NO_NUMBERS_BETWEEN_COMMAS 21
#define CHARS_BEFORE_DATA_ORDER 24
#define COMMA_NO_FOLLOWING_NUMBER 30
/*EXTERN AND ENTRY*/
#define CHARS_BEFORE_EXTERN_OR_ENTRY 25
#define EXTERN_ENTRY_NO_LABEL 33
/*ORDERS*/
#define NO_SUCH_ORDER 22
/*LABELS*/
#define LABEL_TOO_LONG 26
#define MISSING_LABEL 27
#define LABEL_STARTS_WITH_NUMBER 28
#define LABEL_NOT_BY_COLON 29
#define SPACE_IN_LABEL 31
#define LABEL_DOESNT_START_WITH_LETTER 32
#define ILLEGAL_LABEL_SYNTAX 34

/*macros for more elegant code*/
#define PRINT_ERROR_DESCRIPTION puts(line);\
if (print_char_indication == 1){\
va_list argp;\
va_start(argp, counters);\
unexpected = va_arg(argp, int);\
va_end(argp);\
print_visual_indication(unexpected, line);}\
puts("________________________________________________________________________________________________________");

/*~~general functions~~*/
short int errors_inspection(line* sentence, line_marks_index indexes, line_marks_counter* counters);
static void report_error(char* line, short int error_code, line_marks_counter* counters, ...);
/*~~order inspection section~~*/
static short int inspect_order_line(line* sentence, line_marks_index indexes, line_marks_counter* counters);
static short int is_order_proper(line sentence);
static void detect_string_errors(line sentence, line_marks_index indexes, line_marks_counter* counters, short int* error_found);
static void detect_two_quotes_errors(line sentence, line_marks_counter* counters, line_marks_index indexes, short int is_string_order, short int* error_found);
static void detect_data_errors(line sentence, line_marks_index indexes, line_marks_counter* counters, short int* error_found);
static short int is_data_values_proper(line sentence, line_marks_index indexes, line_marks_counter* counters);
static short int inspect_data_values(line sentence, int index, line_marks_counter* counters);
static void values_check(line sentence, line_marks_counter *counters, char curr_char, int last_char, short int is_after_comma,
             short int is_data_order, short int *error_found, short int did_number_appeared, int index);
static void check_if_after_comma(const char *curr_char, short int *is_after_comma);
static void update_loop_data_inspection_variables(line sentence, char* curr_char, char* last_char, int* index);
static void detect_extern_entry_errors(line* sentence, line_marks_index indexes, line_marks_counter* counters, short int* error_found);
static void check_pre_order_chars(line sentence, line_marks_index indexes, line_marks_counter* counters, short int* error_found);
/*~~code inspection section~~*/
static void inspect_code_line(line sentence, line_marks_index indexes, line_marks_counter* counters, short int* error_found);
/*~~label inspection section~~*/
static void inspect_label(line* sentence, line_marks_index indexes, line_marks_counter* counters, int start_index, short int* error_found);
static void check_label_length(line* sentence, line_marks_index indexes, line_marks_counter* counters, int start_index, short int* error_found);
static void check_label_syntax(line sentence, line_marks_index indexes, line_marks_counter* counters, int start_index, short int* error_found);
static void check_pre_label_chars(char* sentence, line_marks_index indexes, line_marks_counter* counters, int start_index, short int* error_found);
#endif /*ASSEMBLER_ERRORS_H*/
