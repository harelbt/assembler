#ifndef ASSEMBLER_PARSER_H
#define ASSEMBLER_PARSER_H
#include "errors.h"

#define REGISTER_CONDITION ((i+1) < str_length && sentence->line[i+1] >= '0' && sentence->line[i+1] <= '9') &&\
((i > 0 && (sentence->line[i-1] == ' ' || sentence->line[i-1] == '\t' ))\
||(i == 0))
/**/
#define COMMENT_CONDITION indexes.semicolon_index == indexes.first_char_index && indexes.semicolon_index >= 0
/**/
#define UNEXPECTED_SEMICOLON_CONDITION indexes.semicolon_index > indexes.first_char_index &&\
indexes.semicolon_index > 0 && indexes.first_char_index >= 0
/**/
#define COLON_CASE if (!colon_found) {\
indexes->colon_index = i;\
colon_found = 1;}\
break;
/**/
#define SEMICOLON_CASE if (!semicolon_found) {\
indexes->semicolon_index = i;\
semicolon_found = 1;}\
break;
/**/
#define HASHMARK_CASE if (number_of_hash_marks < 2) {\
if (!number_of_hash_marks) {\
indexes->first_hash_mark_index = i;\
number_of_hash_marks++;\
break;\
} if (number_of_hash_marks == 1) {\
indexes->second_hash_mark_index = i;\
number_of_hash_marks++;\
break;}} else {\
report_error(sentence, UNEXPECTED_HASHMARK);\
break;}

#define DOT_CASE  if (!dot_found) {\
indexes->dot_index = i;\
dot_found = 1;}\
break;
/**/
#define QUOT_MARK_CASE if (number_of_quotation_marks < 2) {\
if (!number_of_quotation_marks) {\
indexes->first_quotation_mark = i;\
number_of_quotation_marks++;\
break;}\
if (number_of_quotation_marks == 1) {\
indexes->second_quotation_mark = i;\
number_of_quotation_marks++;\
break;}} else {\
report_error(sentence, UNEXPECTED_QUOT_MARK);\
break;}

#define REGISTER_CASE int str_length = (int) strlen(sentence->line);\
if (number_of_registers < 2) {\
if (REGISTER_CONDITION) {\
if (!number_of_registers) {\
indexes->first_register_index = i;\
number_of_registers++;\
break;}\
if (number_of_registers == 1) {\
indexes->second_register_index = i;\
number_of_registers++;}\
break;}\
break;} else {\
report_error(sentence, UNEXPECTED_REGISTER);\
break;}
     /**/
     typedef struct {
         int str_length;
         char operator[4];
         line* sentence;
         int number_of_operators;
         line_marks_index indexes;
     }operator_variables;

 void parse_line (line* sentence);
static void comment_check(line* sentence, line_marks_index indexes);
static void empty_line_check (line* sentence, line_marks_index indexes);
static void find_signs(line* sentence, line_marks_index* indexes);
static void extract_operator(line* sentence, line_marks_index indexes);
static int recognize_operator(char* operator,int* opcode, int* function);
static void find_and_handle_operators(operator_variables* op_variables);
static void handle_operators(operator_variables* op_variables, int recognized_opcode, int recognized_function);
static void define_as_not_instruction(line* sentence);
static void assume_no_signes(line_marks_index* indexes);
void initialize_operator_variables(operator_variables* op_variables, line* sentence, line_marks_index indexes);

#endif //ASSEMBLER_PARSER_H
