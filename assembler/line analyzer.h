#ifndef ASSEMBLER_LINE_ANALYZER_H
#define ASSEMBLER_LINE_ANALYZER_H
#include "assembler data types.h"

/*conditions*/
#define REGISTER_CONDITION ((index+1) < sentence->length && *(sentence->line+index+1) >= '0' && *(sentence->line+index+1) <= '7') &&\
(((index > 0 && ((*(sentence->line+index-1) == ' ' || *(sentence->line+index-1) == '\t' || *(sentence->line+index-1) == ',')))\
||(index == 0)) && counters->number_of_quotation_marks == 0)
/**/
#define OPERATOR_CONDITION (i == 0 || *(sentence->line+i-1) == ' ' || *(sentence->line+i-1) == '\t')\
&& (i == sentence->length-1 || *(sentence->line+i+3) == ' ' || *(sentence->line+i+3) == '\t')\
&& counters->number_of_quotation_marks == 0
#define STOP_OPERATOR_CONDITION ((i == 0 || *(sentence->line+i-1) == ' ' || *(sentence->line+i-1) == '\t')\
&& (i == sentence->length-1 || *(sentence->line+i+4) == ' ' || *(sentence->line+i+4) == '\t' || *(sentence->line+i+4) == '\0')\
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

/*FUNCTIONS DECLARATION*/
/*NON STATIC FUNCTIONS*/
/**
 * analyzes a sentence and stores the components, flags, indexes, counters, etc... in the arguments
 * @param sentence struct line pointer
 * @param indexes struct line_indexes pointer
 * @param counters struct line_counters pointer
 */
void analyze_sentence(line* sentence, line_indexes* indexes, line_counters* counters);
/**
 * checks if the line is a comment or an argument. stores the results in "sentence"
 * @param sentence struct line pointer
 * @param indexes struct line_indexes pointer
 */
void empty_or_comment_line_check (line* sentence, line_indexes* indexes);
/*STATIC FUNCTIONS*/
/*general*/
/**
 * defines if a sentence is code or data (instruction/order). stores the results in "sentence"
 * @param sentence struct line pointer
 * @param counters struct line_counters pointer
 * @param indexes struct line_indexes pointer
 */
void define_sentence_type(line* sentence, line_counters counters, line_indexes indexes);
/**
 * finds and store line components. counts, indexes, etc...
 * @param sentence struct line pointer
 * @param indexes struct line_indexes pointer
 * @param counters struct line_counters pointer
 */
void find_line_components(line* sentence, line_indexes* indexes, line_counters* counters);
/*labels*/
/**
 * sets label's external and sentence type flags. external label gets the label name.
 * in case of external label, requires the index of the label ("data_index").
 * @param sentence struct line pointer
 * @param data_index the index of the label
 */
void prepare_label(line* sentence, int data_index);
/**
 * finds label declaration and stores its name in the "sentence" variable. used in "find_signs" function.
 * this function is built to be used when a colon(':') shows up. if a ' " ' was found before the ':', does'nt do anything
 * because a label can't be declared after a ':'.
 * @param sentence struct line pointer
 * @param indexes struct line_indexes pointer
 */
void find_label(line* sentence, line_indexes indexes);
/*operators*/
/**
 * gets a string, it it's a legal operator returns TRUE, else FALSE.
 * requires opcode and function variables and sets them to the right value.
 * used by "find_line_components" function.
 * @param operator to check
 * @param opcode variable to store the opcode if the operator is rcognized
 * @param function variable to store the opcode if the operator is rcognized
 * @return if "operator" argument is a legal operator returns TRUE, else FALSE.
 */
char recognize_operator(char* operator, int* opcode, int* function);
/**
 * gets an index i and enters the next chars of the code line to the operator_name variables in "sentence" variable.
 * if it's a start of an operator, updates number_of_operators in "counters" variable and operator_index in the "index" variable
 * @param counters struct line_counters pointer
 * @param indexes struct line_indexes pointer
 * @param sentence struct line pointer
 * @param i index of the first char to check
 */
void check_for_operator(line_counters* counters, line_indexes* indexes, line* sentence, int i);
/**
 * initiates an operator recognition check, gets the index to check for operator from.
 * if the index >= line length - 2, does nothing
 * @param sentence struct line pointer
 * @param counters struct line_counters pointer
 * @param indexes struct line_indexes pointer
 * @param index i index of the first char to check
 */
void operator_check(line* sentence, line_counters* counters, line_indexes* indexes, int index);
/*info check*/
/**
 * checks if a char is a sign or 'r' (for registers) and updates relevant flags, counters and indexes in the given arguments.
 * used by the "find_line_components" function and so the flag that are given as arguments*
 * @param sentence struct line pointer
 * @param counters struct line_counters pointer
 * @param indexes struct line_indexes pointer
 * @param curr_char to check
 * @param colon_found flag to set to TRUE or FALSE
 * @param semicolon_found flag to set to TRUE or FALSE
 * @param dot_found flag to set to TRUE or FALSE
 * @param first_char_found flag to set to TRUE or FALSE
 * @param index index of curr char
 */
void signs_check(line* sentence, line_counters* counters, line_indexes* indexes, char curr_char, char* colon_found,
                        char* semicolon_found, char* dot_found, char first_char_found, int index);
/**
 * checks if data was found and if order was ended in an order line and updates the flags that are give as arguments.
 * used by the "find_line_components" function and so the flags that are given as arguments
 * @param indexes struct line_indexes pointer
 * @param order_ended flag to set to TRUE or FALSE
 * @param data_found flag to set to TRUE or FALSE
 * @param dot_found flag to set to TRUE or FALSE
 * @param curr_char current char to check
 * @param index index of curr_char
 */
void data_check(line_indexes* indexes, char* order_ended, char* data_found, char dot_found, char curr_char, int index);
/**
 *checks if the char in the current index is an operand and updates the operands count and indexes.
 * turns "operand_ended" to TRUE if operand has ended in the current index
 * used by the "find_line_components" function and so the flags that are given as arguments
 * @param sentence struct line_indexes pointer
 * @param counters struct line_counters pointer
 * @param indexes struct line_indexes pointer
 * @param operand_ended flag to set to TRUE or FALSE
 * @param curr_char current char to check
 * @param index index of curr_char
 */
void operands_check(line* sentence, line_counters* counters, line_indexes* indexes, char* operand_ended, char curr_char, int index);
/**
 * checks for the first char in the line.
 * used by the "find_line_components" function and so the flags that are given as arguments
 * @param indexes struct line_indexes pointer
 * @param first_char_found flag to set to TRUE or FALSE
 * @param curr_char current char to check
 * @param index index of curr_char
 */
void first_char_check(line_indexes* indexes, char* first_char_found, char curr_char, int index);
/*orders*/
/**
 * given a dot index, gets the order after the dot and enters it to the "order" variable in "sentence".
 * @param sentence struct line_indexes pointer
 * @param dot_index index of the dot before the order
 */
void find_data_order(line* sentence, int dot_index);
/**
 * checks if an order exists in the current code line. returns TRUE if order was found, else FALSE
 * used by the "define_sentence_type" function.
 * @param counters struct line_counters pointer
 * @param indexes struct line_indexes pointer
 * @return
 */
char is_order(line_counters counters, line_indexes indexes);
/**/
#endif /*ASSEMBLER_LINE_ANALYZER_H*/
