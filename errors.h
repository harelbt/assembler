
#ifndef ASSEMBLER_ERRORS_H
#define ASSEMBLER_ERRORS_H
#include "assembler data types.h"

/*information to functions*/
#define SOURCE_OPERAND 1
#define DEST_OPERAND 2

/*ERRORS*/
/*unexpected chars*/
#define UNEXPECTED_CHARACTER 1
/*CODE*/
#define NO_SOURCE_REGISTER_ALLOWED 2
#define NO_DEST_REGISTER_ALLOWED 3
#define EXTRA_OPERATORS 4
#define ZERO_OPERANDS 5
#define EXTRA_OPERANDS 6
#define FEW_OPERANDS 7
#define NO_SOURCE_JUMP_LABEL_ALLOWED 8
#define NO_DEST_JUMP_LABEL_ALLOWED 9
#define NO_DEST_NUMBER_ALLOWED 10
#define NO_SOURCE_NUMBER_ALLOWED 11
#define NOT_AN_OPERAND 12
#define ILLEGAL_CODE_BEFORE_OPERATOR 13
/*SENTENCE TYPES*/
#define MIXED_SENTENCE 14
#define NO_SENTENCE_TYPE 15
/*STRINGS*/
#define SINGLE_QUOTMARK 16
#define STRING_NO_ORDER 17
#define CODE_AFTER_QUOTE 18
#define ORDER_NO_STRING 19
#define CHARS_BETWEEN_ORDER_STRING 20
#define CHARS_BEFORE_STRING_ORDER 21
#define STRING_BEFORE_STRING_ORDER 22
#define NO_SPACE_BEFORE_QUOTE 23
/*DATA*/
#define DATA_NO_ORDER 24
#define ORDER_NO_DATA 25
#define COMMA_NO_NUMBERS 26
#define NO_COMMA_BETWEEN 27
#define NO_NUMBERS_BETWEEN_COMMAS 28
#define CHARS_BEFORE_DATA_ORDER 29
#define COMMA_NO_FOLLOWING_NUMBER 30
#define ARITHMETIC_SIGN_NOT_IN_PLACE 31
/*EXTERN AND ENTRY*/
#define CHARS_BEFORE_EXTERN_OR_ENTRY 32
#define EXTERN_ENTRY_NO_LABEL 33
/*ORDERS*/
#define NO_SUCH_ORDER 34
/*LABELS*/
#define LABEL_TOO_LONG 35
#define MISSING_LABEL 36
#define LABEL_STARTS_WITH_NUMBER 37
#define LABEL_NOT_BY_COLON 38
#define SPACE_IN_LABEL 39
#define LABEL_DOESNT_START_WITH_LETTER 40
#define ILLEGAL_LABEL_SYNTAX 41
#define LABEL_IS_RESERVED 42
#define SECOND_LABEL_DEFINITION 43
/**/
/*macros for more elegant code*/
#define PRINT_ERROR_DESCRIPTION puts(line);\
if (print_char_indication == 1){\
va_list argp;\
va_start(argp, counters);\
unexpected = va_arg(argp, int);\
va_end(argp);\
printf("Starting at the character ' %c '\n", *(line + unexpected));}\
puts("________________________________________________________________________________________________________");
/*conditions*/
#define ILLEGAL_SRC_OPERAND_FIRST_CHAR_CONDITION *(sentence.line + indexes.first_operand_index) != '&' &&\
*(sentence.line + indexes.first_operand_index) != '#' &&\
(*(sentence.line + indexes.first_operand_index) < 'A' ||\
(*(sentence.line + indexes.first_operand_index) > 'Z' && *(sentence.line + indexes.first_operand_index) < 'a')||\
*(sentence.line + indexes.first_operand_index) > 'z')

#define ILLEGAL_DEST_OPERAND_FIRST_CHAR_CONDITION *(sentence.line + indexes.second_operand_index) != '&' && \
*(sentence.line + indexes.second_operand_index) != '#' && \
(*(sentence.line + indexes.second_operand_index) < 'A' || \
(*(sentence.line + indexes.second_operand_index) > 'Z' && *(sentence.line + indexes.second_operand_index) < 'a') || \
*(sentence.line + indexes.second_operand_index) > 'z')

#define ILLEGAL_DATA_CHAR_CONDITION curr_char != ' ' && curr_char != '\t' &&\
curr_char != ',' && curr_char != '-' && curr_char != '+'\
&& (curr_char < '0' || curr_char > '9')

#define ILLEGAL_OPERAND_BODY_CONDITION (*i < '0' || (*i > '9' && *i < 'A') || (*i > 'Z' && *i < 'a') || *i > 'z') && *i != '-' && *i != '+'

#define ILLEGAL_LABEL_CHAR_CONDITION curr_char < '0' || (curr_char > '9' && curr_char < 'A') || (curr_char > 'Z' && curr_char < 'a') || curr_char > 'z'

#define ILLEGAL_LABEL_FIRST_CHAR_CONDITION curr_char < 'A' || (curr_char > 'Z' && curr_char < 'a') || curr_char > 'z'

#define NUMBER_OF_RESERVED_WORDS 27
/*~~general functions~~*/
char errors_inspection(line* sentence, line_marks_index indexes, line_marks_counter* counters);
void report_error(char* line, char error_code, line_marks_counter* counters, ...);
/*~~order inspection section~~*/
static char inspect_order_line(line* sentence, line_marks_index indexes, line_marks_counter* counters);
static short int is_order_proper(line sentence);
static void detect_string_errors(line sentence, line_marks_index indexes, line_marks_counter* counters, char* error_found);
static void detect_two_quotes_errors(line sentence, line_marks_counter* counters, line_marks_index indexes, short int is_string_order, char* error_found);
static void detect_data_errors(line sentence, line_marks_index indexes, line_marks_counter* counters, char* error_found);
static short int is_data_values_proper(line sentence, line_marks_index indexes, line_marks_counter* counters);
static short int inspect_data_values(line sentence, int index, line_marks_counter* counters);
static void values_check(line sentence, line_marks_counter *counters, char curr_char, int last_char, short int is_after_comma,
             short int is_data_order, char* error_found, short int did_number_appeared, int index);
static void check_if_after_comma(const char *curr_char, short int *is_after_comma);
static void update_loop_data_inspection_variables(line sentence, char* curr_char, char* last_char, int* index);
static void detect_extern_entry_errors(line* sentence, line_marks_index indexes, line_marks_counter* counters, char* error_found);
static void check_pre_order_chars(line sentence, line_marks_index indexes, line_marks_counter* counters, char* error_found);
static void check_after_data_chars(line* sentence, line_marks_counter* counters, line_marks_index indexes, char* error_found);
static void inspect_data_label(line* sentence, line_marks_index indexes, line_marks_counter* counters, char* error_found);
/*~~code inspection section~~*/
static void inspect_code_line(line sentence, line_marks_index indexes, line_marks_counter* counters, char* error_found);
static short int check_operands_count(line sentence, line_marks_index indexes, line_marks_counter* counters, char* error_found);
static short int check_operators_count(line sentence, line_marks_index indexes, line_marks_counter* counters, char* error_found);
static void check_operands_syntax(line sentence, line_marks_index indexes, line_marks_counter* counters, char* error_found);
static void check_source_operand_syntax(line sentence, line_marks_index indexes, line_marks_counter* counters, char* error_found);
static void check_dest_operand_syntax(line sentence, line_marks_index indexes, line_marks_counter* counters, char* error_found);
static short int is_operand_proper(line sentence, line_marks_index indexes, short int source_or_dest);
static void check_pre_operator_chars(line sentence, line_marks_index indexes, line_marks_counter* counters, char* error_found);
static void check_operand_body_syntax(char* i, short int* is_propper);
static char* check_operand_first_char_syntax(line sentence, line_marks_index indexes, short int* is_propper,short int source_or_dest);
/*~~label inspection section~~*/
static void inspect_label(line* sentence, line_marks_index indexes, line_marks_counter* counters, int start_index, char* error_found);
static void check_label_length(line* sentence, line_marks_index indexes, line_marks_counter* counters, int start_index, char* error_found);
static void check_label_syntax(line sentence, line_marks_index indexes, line_marks_counter* counters, int start_index, char* error_found);
static void check_if_label_reserved_word(char* label ,line sentence, line_marks_counter* counters, char* error_found);
static void check_label_def_white_chars(line sentence, line_marks_counter* counters, char curr_char, int index, char* error_found);
static void check_label_first_char(line sentence, line_marks_counter* counters, char curr_char, int index, char* error_found);
static char* check_label_body(line sentence, line_marks_counter* counters, char curr_char, int* i, char* error_found);
void print_errors_summary(char* file_name, int errors_count);
#endif /*ASSEMBLER_ERRORS_H*/
