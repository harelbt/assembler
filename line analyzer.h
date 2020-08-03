#ifndef ASSEMBLER_LINE_ANALYZER_H
#define ASSEMBLER_LINE_ANALYZER_H
#include "assembler data types.h"

#define REGISTER_CONDITION ((index+1) < sentence->length && *(sentence->line+index+1) >= '0' && *(sentence->line+index+1) <= '7') &&\
((index > 0 && (*(sentence->line+index-1) == ' ' || *(sentence->line+index-1) == '\t' ))\
||(index == 0) && counters->number_of_quotation_marks == 0)
/**/
#define OPERATOR_CONDITION (i == 0 || *(sentence->line+i-1) == ' ' || *(sentence->line+i-1) == '\t')\
&& (i == sentence->length-1 || *(sentence->line+i+3) == ' ' || *(sentence->line+i+3) == '\t'\
&& counters->number_of_quotation_marks == 0)
#define STOP_OPERATOR_CONDITION (i == 0 || *(sentence->line+i-1) == ' ' || *(sentence->line+i-1) == '\t')\
&& (i == sentence->length-1 || *(sentence->line+i+4) == ' ' || *(sentence->line+i+4) == '\t'\
&& counters->number_of_quotation_marks == 0)
/**/
#define COMMENT_CONDITION indexes->semicolon_index == indexes->first_char_index && indexes->semicolon_index >= 0
/**/
#define COLON_CASE if (!(*colon_found)) {\
indexes->colon_index = index;\
*colon_found = 1;\
find_label(sentence, *indexes);}/*the function gets only labels that come before strings*/\
counters->number_of_colons++;\
break;
/**/
#define SEMICOLON_CASE if (!(*semicolon_found)) {\
indexes->semicolon_index = index;\
if(index == 0 || (first_char_found == 1 && indexes->first_char_index == index)){return;}\
*semicolon_found = 1;}\
break;
/**/
#define HASHMARK_CASE if (counters->number_of_hashmarks < 2) {\
if (!counters->number_of_hashmarks) {\
indexes->first_hash_mark_index = index;\
} else if (counters->number_of_hashmarks == 1) {\
indexes->second_hash_mark_index = index;\
}}\
counters->number_of_hashmarks++;\
break;

#define DOT_CASE  if (!(*dot_found)) {\
indexes->dot_index = index;\
*dot_found = 1;\
find_data_order(sentence, indexes->dot_index);}/*gets the order anyway, because if the order is in a string the line will not be determined as a data line in a later stage*/\
counters->number_of_dots++;\
break;
/**/
#define QUOT_MARK_CASE if (!counters->number_of_quotation_marks) {\
indexes->first_quotation_mark_index = index;}else{\
indexes->second_quotation_mark_index = index;\
} counters->number_of_quotation_marks++;\
break;

#define REGISTER_CASE if (REGISTER_CONDITION) {counters->number_of_registers++;\
if (counters->number_of_registers <= 2) {\
if (counters->number_of_registers == 1) {\
indexes->first_register_index = index;\
} else if (counters->number_of_registers == 2) {\
indexes->second_register_index = index;}}\
}break;
/**/
#define NUMBER_OF_OPERATORS 15
void empty_or_comment_line_check (line* sentence, line_indexes* indexes);
void analyze_sentence(line* sentence, line_indexes* indexes, line_counters* counters);
static int recognize_operator(char* operator, int* opcode, int* function);
static void check_for_operators(line_counters* counters, line_indexes* indexes, line* sentence, int i);
static void find_line_components(line* sentence, line_indexes* indexes, line_counters* counters);
void prepare_label(line* sentence, int data_index);
static void signs_check(line* sentence, line_counters* counters, line_indexes* indexes, char curr_char, char* colon_found,
                        char* semicolon_found, char* dot_found, char first_char_found, int index);
static void data_check(line_indexes* indexes, char* order_ended, char* data_found, char dot_found, char curr_char, int index);
static void operands_check(line* sentence, line_counters* counters, line_indexes* indexes, char* operand_ended, char curr_char, int index);
static void operator_check(line* sentence, line_counters* counters, line_indexes* indexes, int index);
static void first_char_check(line_indexes* indexes, char* first_char_found, char curr_char, int index);
void find_data_order(line* sentence, int dot_index);
static void define_sentence_type(line* sentence, line_counters counters, line_indexes indexes);
static short int is_order(line_counters counters, line_indexes indexes);
static void find_label(line* sentence, line_indexes indexes);
#endif //ASSEMBLER_LINE_ANALYZER_H
