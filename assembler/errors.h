
#ifndef ASSEMBLER_ERRORS_H
#define ASSEMBLER_ERRORS_H
#include "assembler data types.h"

/*functions modes*/
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
#define NO_COMMAS_ALLOWED 14
#define EXTRA_COMMAS 15
#define COMMA_NOT_IN_PLACE 16
#define COMMA_REQUIRED 17
/*SENTENCE TYPES*/
#define MIXED_SENTENCE 18
#define NO_SENTENCE_TYPE 19
/*STRINGS*/
#define SINGLE_QUOTMARK 20
#define STRING_NO_ORDER 21
#define CODE_AFTER_QUOTE 22
#define ORDER_NO_STRING 23
#define CHARS_BETWEEN_ORDER_STRING 24
#define CHARS_BEFORE_STRING_ORDER 25
#define STRING_BEFORE_STRING_ORDER 26
#define NO_SPACE_BEFORE_QUOTE 27
/*DATA*/
#define DATA_NO_ORDER 28
#define ORDER_NO_DATA 29
#define COMMA_NO_NUMBERS 30
#define NO_COMMA_BETWEEN 31
#define NO_NUMBERS_BETWEEN_COMMAS 32
#define CHARS_BEFORE_DATA_ORDER 33
#define COMMA_NO_FOLLOWING_NUMBER 34
#define ARITHMETIC_SIGN_NOT_IN_PLACE 35
#define NUMBER_TOO_LARGE 36
#define SIGN_NOT_BY_NUM 37
/*EXTERN AND ENTRY*/
#define CHARS_BEFORE_EXTERN_OR_ENTRY 38
#define EXTERN_ENTRY_NO_LABEL 39
/*ORDERS*/
#define NO_SUCH_ORDER 40
/*LABELS*/
#define LABEL_TOO_LONG 41
#define MISSING_LABEL 42
#define LABEL_STARTS_WITH_NUMBER 43
#define LABEL_NOT_BY_COLON 44
#define SPACE_IN_LABEL 45
#define LABEL_DOESNT_START_WITH_LETTER 46
#define ILLEGAL_LABEL_SYNTAX 47
#define LABEL_IS_RESERVED 48
#define SECOND_LABEL_DEFINITION 49
#define ENTRY_NOT_EXIST 50
#define LABEL_DOESNT_EXIST 51
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
#define ILLEGAL_SRC_OPERAND_FIRST_CHAR_CONDITION *(sentence->line + indexes->first_operand_index) != '&' &&\
*(sentence->line + indexes->first_operand_index) != '#' &&\
(*(sentence->line + indexes->first_operand_index) < 'A' ||\
(*(sentence->line + indexes->first_operand_index) > 'Z' && *(sentence->line + indexes->first_operand_index) < 'a')||\
*(sentence->line + indexes->first_operand_index) > 'z')

#define ILLEGAL_DEST_OPERAND_FIRST_CHAR_CONDITION *(sentence->line + indexes->second_operand_index) != '&' && \
*(sentence->line + indexes->second_operand_index) != '#' && \
(*(sentence->line + indexes->second_operand_index) < 'A' || \
(*(sentence->line + indexes->second_operand_index) > 'Z' && *(sentence->line + indexes->second_operand_index) < 'a') || \
*(sentence->line + indexes->second_operand_index) > 'z')

#define ILLEGAL_DATA_CHAR_CONDITION curr_char != ' ' && curr_char != '\t' &&\
curr_char != ',' && curr_char != '-' && curr_char != '+'\
&& (curr_char < '0' || curr_char > '9')

#define ILLEGAL_OPERAND_BODY_CONDITION (*i < '0' || (*i > '9' && *i < 'A') || (*i > 'Z' && *i < 'a') || *i > 'z') && *i != '-' && *i != '+'

#define ILLEGAL_LABEL_CHAR_CONDITION curr_char < '0' || (curr_char > '9' && curr_char < 'A') || (curr_char > 'Z' && curr_char < 'a') || curr_char > 'z'

#define ILLEGAL_LABEL_FIRST_CHAR_CONDITION curr_char < 'A' || (curr_char > 'Z' && curr_char < 'a') || curr_char > 'z'
/*GENERAL*/
#define NUMBER_OF_RESERVED_WORDS 27
#define NUMBER_MAX_VAL 1048575 /*0b011111111111111111111*/
/*FUNCTIONS DECLARATION*/
/*~~general functions~~*/
/**
 * inspects an assembly line for all kinds of errors. returns TRUE if errors found, else returns FALSE
 * @param sentence struct line pointer
 * @param indexes struct line_index pointer
 * @param counters struct line_counters pointer
 * @return TRUE if errors found, else returns FALSE
 */
char errors_inspection(line* sentence, line_indexes* indexes, line_counters* counters);
/**
 * prints an error to stdout with error number, line number, a string of the line and in most cases, a char which where the error occurs-
 * this requires extra optional argument. this argument is the char index and it's required in most cases and useless in others
 * requires error code. 51 error codes are defined as macros in errors.h
 * @param line - char* of the line of the error
 * @param error_code macro of the error
 * @param counters line counters pointer to supply the line number and the error number
 * @param ... extra optional argument for char indication which where the error occurs
 */
void report_error(char* line, char error_code, line_counters* counters, ...);
/**
 * checks if the "curr_char" is a number, if so, turns "did_number_appeared" char to TRUE. used by "inspect_data_values" function
 * @param did_number_appeared a flag to store the result in it
 * @param curr_char to check
 */
void check_number_appearance(char* did_number_appeared, char curr_char);
/*~~order inspection section~~*/
/**
 *inspects order line, from the order itself to the data. if errors found, turns "error_found" to TRUE, else FALSE
 * @param sentence struct line pointer
 * @param indexes struct line_indexes pointer
 * @param counters struct line_counters pointer
 * @param error_found error flag. if errors found, turns to TRUE, else FALSE
 */
void inspect_order_line(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found);
/**
 * checks if the order in the line (.data/.string/.extern/.entry) is proper, returns TRUE if proper, else FALSE
 * @param sentence struct line pointer
 * @return returns TRUE if order is proper, else FALSE
 */
char is_order_proper(line* sentence);
/**
 * detects and reports string errors. error flag. if errors found, turns to TRUE, else FALSE
 * @param sentence struct line pointer
 * @param indexes struct line_indexes pointer
 * @param counters struct line_counters pointer
 * @param error_found error flag. if errors found, turns to TRUE, else FALSE
 */
void detect_string_errors(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found);
/**
 * detects errors in a line with with a proper string. requires "is_order" flag to tell the function if .string exists.  TRUE if exists, FALSE
 * @param sentence struct line pointer
 * @param counters struct line_counters pointer
 * @param indexes struct line_indexes pointer
 * @param is_string_order flag to tell the function if .string exists. TRUE if exists, FALSE
 * @param error_found error flag. if errors found, turns to TRUE, else FALSE
 */
void detect_two_quotes_errors(line* sentence, line_counters* counters, line_indexes* indexes, char is_string_order, char* error_found);
/**
 *detects .data order line errors. if errors found, turns "error_found" to TRUE, else FALSE
 * @param sentence struct line pointer
 * @param indexes line_indexes pointer
 * @param counters line_counters pointer
 * @param error_found error flag. if errors found, turns to TRUE, else FALSE
 */
void detect_data_errors(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found);
/**
 * checks if data values are proper. for example (2,4,6). if errors found, returns TRUE, else FALSE
 * @param sentence struct line pointer
 * @param indexes line_indexes pointer
 * @param counters line_counters pointer
 * @return TRUE is data values proper, else FALSE
 */
char is_data_values_proper(line* sentence, line_indexes* indexes, line_counters* counters);
/**
 *inspects the data values for example (2,3,8), if errors found returns TRUE else FALSE
 * @param sentence struct line pointer
 * @param index line_indexes pointer
 * @param counters line_counters pointer
 * @return TRUE is data values proper, else FALSE
 */
char inspect_data_values(line* sentence, int index, line_counters* counters);
/**
 * data values check for "inspect_data_values" function. if errors found, turns "error_found" to TRUE, else FALSE
 * @param sentence struct line pointer
 * @param counters line_indexes pointer
 * @param curr_char current char to check
 * @param last_char char before curr_char
 * @param is_after_comma needs to be TRUE if after comma, else FALSE
 * @param is_data_order needs to be TRUE if .data exists, else FALSE
 * @param error_found error flag. if errors found, turns to TRUE, else FALSE
 * @param did_number_appeared  needs to be TRUE if number already appeared, else FALSE
 * @param index index of curr_char argument
 */
void values_check(line* sentence, line_counters *counters, char curr_char, int last_char, short int is_after_comma,
                         char is_data_order, char* error_found, short int did_number_appeared, int index);
/**
 * turns on and off "is_after_comma" flag for "inspect_data_values" function.
 * turns to TRUE if curr_char is ',' and FALSE if curr_char is not a white char.
 * @param curr_char to check
 * @param is_after_comma flag. turns to TRUE if curr_char is ',' and FALSE if curr_char is not a white char
 */
void check_if_after_comma(const char *curr_char, char* is_after_comma);
/**
 * updates loop variables for "inspect_data_values" function
 * @param sentence struct line pointer
 * @param curr_char current inspected char
 * @param last_char char before curr_char
 * @param index index of curr_char
 */
void update_loop_data_inspection_variables(line* sentence, char* curr_char, char* last_char, int* index);
/**
 * checks errors for .entry and .extern lines. if errors found, turns "error_found" to TRUE, else FALSE
 * @param sentence struct line pointer
 * @param indexes line_indexes pointer
 * @param counters line_counters pointer
 * @param error_found if errors found, turns to TRUE, else FALSE
 */
void detect_extern_entry_errors(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found);
/**
 *checks for illegal characters before an order in an order line. if errors found, turns "error_found" to TRUE, else FALSE
 * @param sentence struct line pointer
 * @param indexes line_indexes pointer
 * @param counters line_counters pointer
 * @param error_found if errors found, turns to TRUE, else FALSE
 */
void check_pre_order_chars(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found);
/**
 * checks and reports if there are characters after a label in a .extern ot .entry line. if errors found, turns "error_found" to TRUE, else FALSE
 * @param sentence struct line pointer
 * @param counters line_counters pointer
 * @param indexes line_indexes pointer
 * @param error_found if errors found, turns to TRUE, else FALSE
 */
void check_after_data_label_chars(line* sentence, line_counters* counters, line_indexes* indexes, char* error_found);
/**
 * inspects a label that comes as data after .extern or .entry. if errors found, turns "error_found" to TRUE, else FALSE
 * @param sentence struct line pointer
 * @param indexes line_indexes pointer
 * @param counters line_counters pointer
 * @param error_found if errors found, turns to TRUE, else FALSE
 */
void inspect_data_label(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found);
/*~~code inspection section~~*/
/**
 * checks a code line for errors. if errors found, turns "error_found" to TRUE, else FALSE
 * @param sentence struct line pointer
 * @param indexes line_indexes pointer
 * @param counters line_counters pointer
 * @param error_found if errors found, turns "error_found" to TRUE, else FALSE
 */
void inspect_code_line(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found);
/**
 * checks if the number of operands is legal for the operator, reports errors. returns TRUE if error found, else returns FALSE
 * if errors found, turns "error_found" to TRUE, else FALSE
 * @param sentence struct line pointer
 * @param indexes line_indexes pointer
 * @param counters line_counters pointer
 * @param error_found if errors found, turns "error_found" to TRUE, else FALSE
 * @return returns TRUE if error found, else returns FALSE
 */
char check_operands_count(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found);
/**
 * checks if the number of operators is greater than 1, if so reports an error and returns TRUE. else returns FALSE
 * if errors found, turns "error_found" to TRUE, else FALSE
 * @param sentence struct line pointer
 * @param indexes line_indexes pointer
 * @param counters line_counters pointer
 * @param error_found if errors found, turns "error_found" to TRUE, else FALSE
 * @return if errors found, returns TRUE, else FALSE
 */
char check_operators_count(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found);
/**
 * checks operands syntax according to the operator and commas conditions, reports errors
 * if errors found, turns "error_found" to TRUE, else FALSE
 * @param sentence struct line pointer
 * @param indexes struct line_indexes pointer
 * @param counters struct line_counters pointer
 * @param error_found if errors found, turns "error_found" to TRUE, else FALSE
 */
void check_operands_syntax(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found);
/**
 * gets a number in char*, number can be with a sign, and reports an error if number is too large
 * if errors found, turns "error_found" to TRUE, else FALSE
 * @param line struct line pointer
 * @param index struct line_indexes pointer
 * @param counters struct line_counters pointer
 * @param error_found if errors found, turns "error_found" to TRUE, else FALSE
 */
void check_number_size(char* line, int index, line_counters* counters, char* error_found);
/**
 * checks source operand syntax and reports errors. if errors found, turns "error_found" to TRUE, else FALSE
 * @param sentence struct line pointer
 * @param indexes struct line_indexes pointer
 * @param counters struct line_counters pointer
 * @param error_found if errors found, turns "error_found" to TRUE, else FALSE
 */
void check_source_operand_syntax(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found);
/**
 * checks destination operand syntax and reports errors. if errors found, turns "error_found" to TRUE, else FALSE
 * @param sentence struct line pointer
 * @param indexes line_indexes pointer
 * @param counters line_counters pointer
 * @param error_found  if errors found, turns "error_found" to TRUE, else FALSE
 */
void check_dest_operand_syntax(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found);
/**
 * returns TRUE if operand is proper, else returns FALSE, requires SOURCE_OPERAND or
 * SOURCE_OPERAND macros to know which operand to check
 * @param sentence struct line pointer
 * @param indexes line_indexes pointer
 * @param source_or_dest SOURCE_OPERAND or SOURCE_OPERAND macros to know which operand to check
 * @return TRUE if operand is proper, else returns FALSE
 */
char is_operand_proper(line* sentence, line_indexes* indexes, char source_or_dest);
/**
 * checks the char that come before the operator. if errors found, turns "error_found" to TRUE, else FALSE
 * @param sentence struct line pointer
 * @param indexes line_indexes pointer
 * @param counters line_counters pointer
 * @param error_found if errors found, turns "error_found" to TRUE, else FALSE
 */
void check_pre_operator_chars(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found);
/**
 * checks the syntax of the operand's body. sets the "is_propper" flag that is given as an argument.
 * @param i char* to the operand's body
 * @param is_proper turns to TRUE if proper, else FALSE
 */
void check_operand_body_syntax(char* i, char* is_proper);
/**
 * checks the syntax of the first char of the operand, sets the "is_propper" flag that is given as an argument.
 * requires source_or_dest flag with macros: SOURCE_OPERAND, DEST_OPERAND. due to different checks.
 * returns the line from the second char of the operand
 * @param sentence struct line pointer
 * @param indexes line_indexes pointer
 * @param is_proper turns to TRUE if proper, else FALSE
 * @param source_or_dest SOURCE_OPERAND, DEST_OPERAND. according to the required operand check
 * @return the line from the second char of the operand
 */
char* check_operand_first_char_syntax(line* sentence, line_indexes* indexes, char* is_proper, char source_or_dest);
/*~~label inspection section~~*/
/**
 * inspects labels in a line. if errors found, turns "error_found" to TRUE, else FALSE
 * @param sentence struct line pointer
 * @param indexes struct line_indexes pointer
 * @param counters struct line_counters pointer
 * @param start_index label's start length
 * @param error_found if errors found, turns "error_found" to TRUE, else FALSE
 */
void inspect_label(line* sentence, line_indexes* indexes, line_counters* counters, int start_index, char* error_found);
/**
 * checks if the length of the label is legal (1-31) and reports an error if needed. requires label start index
 * if errors found, turns "error_found" to TRUE, else FALSE
 * @param sentence line pointer
 * @param indexes line_indexes pointer
 * @param counters line_counters pointer
 * @param start_index label's start length
 * @param error_found if errors found, turns "error_found" to TRUE, else FALSE
 */
void check_label_length(line* sentence, line_indexes* indexes, line_counters* counters, int start_index, char* error_found);
/**
 * checks a label syntax, requires label start index. if errors found, turns "error_found" to TRUE, else FALSE
 * @param sentence struct line pointer
 * @param indexes struct line_indexes pointer
 * @param counters struct line_counters pointer
 * @param start_index label's start length
 * @param error_found if errors found, turns "error_found" to TRUE, else FALSE
 */
void check_label_syntax(line* sentence, line_indexes* indexes, line_counters* counters, int start_index, char* error_found);
/**
 * checks if the label is a reserved word (it's illegal). if errors found, turns "error_found" to TRUE, else FALSE
 * @param label char* label's name to compare to reserved words
 * @param counters struct line_counters pointer
 * @param error_found if errors found, turns "error_found" to TRUE, else FALSE
 */
void check_if_label_reserved_word(char* label, line_counters* counters, char* error_found);
/**
 * checks if the curr char is a white char, used for inspecting if a white char is between a label definition and the colon(':')
 * afterwards by, used "check_label_syntax" function
 * if errors found, turns "error_found" to TRUE, else FALSE
 * @param sentence struct line pointer
 * @param counters struct line_counters pointer
 * @param curr_char the current char to check
 * @param index index of the curr_char
 * @param error_found if errors found, turns "error_found" to TRUE, else FALSE
 */
void check_label_def_white_chars(line* sentence, line_counters* counters, char curr_char, int index, char* error_found);
/**\
 * checks the syntax of the label's first char. used by "check_label_syntax" function. if errors found, turns "error_found" to TRUE, else FALSE
 * @param sentence struct line pointer
 * @param counters struct line_counters pointer
 * @param curr_char current char to check
 * @param index index of curr_char
 * @param error_found if errors found, turns "error_found" to TRUE, else FALSE
 */
void check_label_first_char(line* sentence, line_counters* counters, char curr_char, int index, char* error_found);
/**
 * checks the syntax of the label's body, returns a copy of the label. used by "check_label_syntax" function.
 * if errors found, turns "error_found" to TRUE, else FALSE
 * @param sentence struct line pointer
 * @param counters struct line_counters pointer
 * @param curr_char current char index
 * @param i int* curr char index
 * @param error_found if errors found, turns "error_found" to TRUE, else FALSE
 * @return a copy of the label
 */
char* check_label_body(line* sentence, line_counters* counters, char curr_char, int* i, char* error_found);
/**/
#endif /*ASSEMBLER_ERRORS_H*/
