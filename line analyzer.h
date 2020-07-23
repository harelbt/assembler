#ifndef ASSEMBLER_LINE_ANALYZER_H
#define ASSEMBLER_LINE_ANALYZER_H
#include "assembler data types.h"

#define REGISTER_CONDITION ((i+1) < sentence->length && *(sentence->line+i+1) >= '0' && *(sentence->line+i+1) <= '9') &&\
((i > 0 && (*(sentence->line+i-1) == ' ' || *(sentence->line+i-1) == '\t' ))\
||(i == 0) && counters->number_of_quotation_marks == 0)
/**/
#define OPERATOR_CONDITION (i == 0 || *(line+i-1) == ' ' || *(line+i-1) == '\t')\
&& (i == str_length-1 || *(line+i+3) == ' ' || *(line+i+3) == '\t'\
&& counters->number_of_quotation_marks == 0)
/**/
#define COMMENT_CONDITION indexes.semicolon_index == indexes.first_char_index && indexes.semicolon_index >= 0
/**/
#define UNEXPECTED_SEMICOLON_CONDITION indexes.semicolon_index > indexes.first_char_index &&\
indexes.semicolon_index > 0 && indexes.first_char_index >= 0
/**/
#define COLON_CASE if (!colon_found) {\
indexes->colon_index = i;\
colon_found = 1;\
find_label(sentence, *indexes);}/*the function gets only labels that come before strings*/\
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
dot_found = 1;\
find_data_order(sentence, indexes->dot_index);}/*gets the order anyway, because if the order is in a string the line will not be determined as a data line in a later stage*/\
counters->number_of_dots++;\
break;
/**/
#define QUOT_MARK_CASE if (!counters->number_of_quotation_marks) {\
indexes->first_quotation_mark_index = i;}else{\
indexes->second_quotation_mark_index = i;\
} counters->number_of_quotation_marks++;\
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
static void check_for_operators(operator* op_variables, line_marks_counter* counters, line_marks_index* indexes, char * line, int i);
static void find_line_components(line* sentence, line_marks_index* indexes, line_marks_counter* counters, operator* op_variables);
void find_data_order(line* sentence, int dot_index);
static void define_sentence_type(line* sentence, line_marks_counter counters, line_marks_index indexes, operator op_variables);
static short int is_order(line_marks_counter counters,line_marks_index indexes);
static void define_as_order(line* sentence);
static void define_as_not_order(line* sentence);
static void find_label(line* sentence, line_marks_index indexes);
#endif //ASSEMBLER_LINE_ANALYZER_H
