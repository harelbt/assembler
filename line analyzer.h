#ifndef ASSEMBLER_LINE_ANALYZER_H
#define ASSEMBLER_LINE_ANALYZER_H
#include "assembler data types.h"

#define REGISTER_CONDITION ((i+1) < str_length && *(line_pointer+i+1) >= '0' && *(line_pointer+i+1) <= '9') &&\
((i > 0 && (*(line_pointer+i-1) == ' ' || *(line_pointer+i-1) == '\t' ))\
||(i == 0) && counters->number_of_quotation_marks == 0)
/**/
#define OPERATOR_CONDITION (i == 0 || *(line_pointer+i-1) == ' ' || *(line_pointer+i-1) == '\t')\
&& (i == str_length-1 || *(line_pointer+i+3) == ' ' || *(line_pointer+i+3) == '\t'\
&& counters->number_of_quotation_marks == 0)
/**/
#define COMMENT_CONDITION indexes.semicolon_index == indexes.first_char_index && indexes.semicolon_index >= 0
/**/
#define UNEXPECTED_SEMICOLON_CONDITION indexes.semicolon_index > indexes.first_char_index &&\
indexes.semicolon_index > 0 && indexes.first_char_index >= 0
/**/
#define COLON_CASE if (!colon_found) {\
indexes->colon_index = i;\
colon_found = 1;}\
counters->number_of_colons++;\
break;
/**/
#define SEMICOLON_CASE if (!semicolon_found) {\
indexes->semicolon_index = i;\
if(i == 0 || (first_char_found == 1 && indexes->first_char_index == i)){return;}\
semicolon_found = 1;}\
break;
/**/
#define HASHMARK_CASE if (counters->number_of_hashmarks < 2) {\
if (!counters->number_of_hashmarks) {\
indexes->first_hash_mark_index = i;\
} else if (counters->number_of_hashmarks == 1) {\
indexes->second_hash_mark_index = i;\
}}\
counters->number_of_hashmarks++;\
break;

#define DOT_CASE  if (!dot_found) {\
indexes->dot_index = i;\
dot_found = 1;}\
counters->number_of_dots++;\
break;
/**/
#define QUOT_MARK_CASE if (counters->number_of_quotation_marks < 2) {\
if (!counters->number_of_quotation_marks) {\
indexes->first_quotation_mark_index = i;}\
else{\
indexes->second_quotation_mark_index = i;\
}} counters->number_of_quotation_marks++;\
break;

#define REGISTER_CASE if (REGISTER_CONDITION) {counters->number_of_registers++;\
if (counters->number_of_registers <= 2) {\
if (counters->number_of_registers == 1) {\
indexes->first_register_index = i;\
} else if (counters->number_of_registers == 2) {\
indexes->second_register_index = i;}}\
}break;
/**/

void comment_check(line* sentence, line_marks_index indexes);
void empty_line_check (line* sentence, line_marks_index indexes);
void analyze_sentence(line* sentence, line_marks_index* indexes, line_marks_counter* counters);
static int recognize_operator(char* operator, int* opcode, int* function);
static void define_as_not_code(line* sentence);
static void define_as_code(line* sentence, int opcode, int function);
static void assume_no_signs(line_marks_index* indexes, line_marks_counter* counters);
void check_for_operators(operator* op_variables, line_marks_counter* counters, char * line_pointer, int i);
void find_line_components(line* sentence, line_marks_index* indexes, line_marks_counter* counters, operator* op_variables);
void define_sentence_type(line* sentence, line_marks_counter counters, line_marks_index indexes, operator op_variables);
short int is_order(line_marks_counter counters,line_marks_index indexes);
void define_as_order(line* sentence);
void define_as_not_order(line* sentence);
#endif //ASSEMBLER_LINE_ANALYZER_H