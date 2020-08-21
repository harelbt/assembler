#include <stdlib.h>
#include <string.h>
#include "line analyzer.h"
/*--for the first pass--*/
/*NON STATIC FUNCTIONS*/
/*
 * analyzes a sentence and stores the components, flags, indexes, counters, etc... in the arguments
 */
void analyze_sentence(line* sentence, line_indexes* indexes, line_counters* counters) {
    find_line_components(sentence, indexes, counters);/*fills sentence, indexes and counters*/
    define_sentence_type(sentence, *counters, *indexes);/*code or order*/
    prepare_label(sentence, indexes->data_index);
}
/*
 * checks if the line is a comment or an argument. stores the results in "sentence"
 */
void empty_or_comment_line_check (line* sentence, line_indexes* indexes){
    /*empty line check*/
    if (indexes->first_char_index == NOT_FOUND) {
        sentence->flags.is_empty_line = TRUE;
    }
    /*comment check*/
    if (COMMENT_CONDITION){
        sentence->flags.is_comment = TRUE;
        return;
    }
}
/*STATIC FUNCTIONS*/
/*general*/
/*
 * defines if a sentence is code or data (instruction/order). stores the results in "sentence"
 */
void define_sentence_type(line* sentence, line_counters counters, line_indexes indexes) {
    if (counters.number_of_operators > 0) {/*operators makes the sentence an instruction line*/
        sentence->flags.is_code = TRUE;
    }
    if (is_order(counters, indexes) == TRUE) {/*order makes the sentence an orde line*/
        sentence->flags.is_data = TRUE;
    }
}
/*
 * finds and store line components. counts, indexes, etc...
 */
void find_line_components(line* sentence, line_indexes* indexes, line_counters* counters) {
    /*flags for components*/
    char colon_found = FALSE;
    char semicolon_found = FALSE;
    char dot_found = FALSE;
    char first_char_found = FALSE;
    char operand_ended = FALSE;
    char order_ended = FALSE;
    char data_found = FALSE;
    /**/
    int i = 0;
    char curr_char;
    /*every char in the line goes through checks*/
    while (i < sentence->length) {/*while the string isn't ended*/
        curr_char = *(sentence->line + i);
        first_char_check(indexes, &first_char_found, curr_char, i);/*turns on the first_char_found flag*/
        operator_check(sentence, counters, indexes, i);/*finds the operator if it starts from curr char*/
        operands_check(sentence, counters, indexes, &operand_ended, curr_char, i);/*finds an operand if it starts from curr char*/
        data_check(indexes, &order_ended, &data_found, dot_found, curr_char, i);/*sets the order_ended, data_found flags*/
        signs_check(sentence, counters, indexes, curr_char, &colon_found, &semicolon_found, &dot_found,
                    first_char_found, i);/*updates the indexes and counters according to the curr char (registers, colons, etc...)*/
        /*comment line condition, no reason to continue*/
        if (curr_char == ';' && indexes->first_char_index == i) { return; }
        i++;
    }
    /*if only one operand was found, set it as the second operand (destination operand)*/
    if (indexes->first_operand_index != NOT_FOUND && indexes->second_operand_index == NOT_FOUND) {
        indexes->second_operand_index = indexes->first_operand_index;
        indexes->first_operand_index = NOT_FOUND;
    }
}
/*labels*/
/*
 * sets label's external and sentence type flags. external label gets the label name.
 * in case of external label, requires the index of the label ("data_index").
 */
void prepare_label(line* sentence, int data_index){
    if (sentence->flags.is_code == TRUE) {
        /*prepares sentence type*/
        sentence->label.external = FALSE;
        sentence->label.sentence_type = CODE;
    }else{
        sentence->label.sentence_type = DATA;
        if (!strcmp(sentence->data_parts.order, "extern")) {
            /*prepares sentence type*/
            sentence->label.external = EXTERN;
            /*prepares label's name*/
            strncpy(sentence->label.name, sentence->line + data_index, LABEL_MAX_LENGTH);
        }
    }
}
/*
 * finds label declaration and stores its name in the "sentence" variable. used in "find_signs" function.
 * this function is built to be used when a colon(':') shows up. if a ' " ' was found before the ':', does'nt do anything
 * because a label can't be declared after a ':'.
 */
void find_label(line* sentence, line_indexes indexes){
    /*if a ' " ' was found before the ':' it's not a label definition*/
    if (indexes.first_quotation_mark_index == NOT_FOUND) {
        int i = 0;
        /*enters label's name to in the "sentence" variable.*/
        /*LABEL_MAX_LENGTH == 32, so max index is 30 and index 31 is '\0' (LABEL_MAX_LENGTH - 2)*/
        while ((i + indexes.first_char_index < indexes.colon_index) && i < LABEL_MAX_LENGTH - 1) {
            *(sentence->label.name + i) = *(sentence->line + (i + indexes.first_char_index));
            i++;
        }
        *(sentence->label.name + i) = '\0';/*end of string*/
    }
}
/*operators*/
/*
 * gets a string, it it's a legal operator returns TRUE, else FALSE.
 * requires opcode and function variables and sets them to the right value.
 * used by "find_line_components" function
 */
char recognize_operator(char* operator, int* opcode, int* function){
    int i = 0;
    /*operators list*/
    char* operators_table[NUMBER_OF_OPERATORS][3] = {{"mov", "0", ""},{"cmp", "1", ""},{"add", "2", "1"},{"sub", "2", "2"},{"lea", "4", ""},
                                                     {"clr", "5", "1"},{"not", "5", "2"},{"inc", "5", "3"},{"dec", "5", "4"},{"jmp", "9", "1"},
                                                     {"bne","9", "2"},{"jsr", "9", "3"},{"red", "12", ""},{"prn", "13", ""},{"rts", "14", ""}
    };
    while (i < NUMBER_OF_OPERATORS){
        if (!strcmp(operator, operators_table[i][0])){/*if operator was found, gets the opcode and the function*/
            *opcode = strtod(operators_table[i][1],NULL);
            *function = strtod(operators_table[i][2],NULL);
            return TRUE;
        }
        i++;
    }
    return FALSE;/*operator wasn't found*/
}
/*
 * gets an index i and enters the next chars of the code line to the operator_name variables in "sentence" variable.
 * if it's a start of an operator, updates number_of_operators in "counters" variable and operator_index in the "index" variable
 */
void check_for_operator(line_counters* counters, line_indexes* indexes, line* sentence, int i){
    /*enters the next 3 chars to the "operator_name" variable*/
    *sentence->code_parts.operator_name = *(sentence->line + i);
    *(sentence->code_parts.operator_name+1) = *(sentence->line + i + 1);
    *(sentence->code_parts.operator_name+2) = *(sentence->line + i + 2);
    /*if the line length allows it, enters the fourth char to check if it's a "stop" operator*/
    if (i < sentence->length-3){
        *(sentence->code_parts.operator_name+3) = *(sentence->line + i + 3);
        *(sentence->code_parts.operator_name+4) = '\0';
        /*if it's a "stop" operator, updates the relevant variables*/
        if (!strcmp(sentence->code_parts.operator_name,"stop") && STOP_OPERATOR_CONDITION){
            sentence->code_parts.opcode = 15;
            counters->number_of_operators++;
            return;
            /*if it's not a "stop" operator, enters a '\0' instead of the fourth char and checks if it's another operator
             * if so updates the relevant variables*/
        } else{
            *(sentence->code_parts.operator_name+3) = '\0';
        }
    }
    if (recognize_operator(sentence->code_parts.operator_name, &sentence->code_parts.opcode, &sentence->code_parts.function) == TRUE
        && OPERATOR_CONDITION){
        counters->number_of_operators++;
        indexes->operator_index = i;
        return;
    }
}
/*
 * initiates an operator recognition check, gets the index to check for operator from.
 * if the index >= line length - 2, does nothing
 */
void operator_check(line* sentence, line_counters* counters, line_indexes* indexes, int index){
    if (index < sentence->length - 2){
        check_for_operator(counters, indexes, sentence, index);
    }
}
/*info check*/
/*checks if a char is a sign or 'r' (for registers) and updates relevant flags, counters and indexes in the given arguments.
 * used by the "find_line_components" function and so the flag that are given as arguments*/
void signs_check(line* sentence, line_counters* counters, line_indexes* indexes, char curr_char, char* colon_found,
                        char* semicolon_found, char* dot_found, char first_char_found, int index){
    /*updates relevant flags, counters and indexes in the given arguments.*/
    switch (curr_char) {
        case ':': {
            COLON_CASE
        }
        case ';': {
            SEMICOLON_CASE
        }
        case '.': {
            DOT_CASE
        }
        case 'r': {
            REGISTER_CASE
        }
        case '\"': {
            QUOT_MARK_CASE
        }
        case ',':{
            counters->number_of_commas++;
            indexes->last_comma_index = index;
        }
    }
}
/*
 * checks if data was found and if order was ended in an order line and updates the flags that are give as arguments.
 * used by the "find_line_components" function and so the flags that are given as arguments
 */
void data_check(line_indexes* indexes, char* order_ended, char* data_found, char dot_found, char curr_char, int index){
    /*if order ended turns "order_ended" to TRUE*/
    if (!(*order_ended) && dot_found == TRUE && (curr_char == ' ' || curr_char == '\t')){
        *order_ended = TRUE;
    }
    /*if data was found turns "order_ended" to TRUE*/
    if (*order_ended == TRUE && dot_found == TRUE && !(*data_found) && curr_char != ' ' && curr_char != '\t'){
        indexes->data_index = index;
        *data_found = TRUE;
    }
}
/*
 * checks if the char in the current index is an operand and updates the operands count and indexes.
 * turns "operand_ended" to TRUE if operand has ended in the current index
 * used by the "find_line_components" function and so the flags that are given as arguments
 */
void operands_check(line* sentence, line_counters* counters, line_indexes* indexes, char* operand_ended, char curr_char, int index){
    if (sentence->code_parts.opcode != NOT_FOUND){/*if an operand can exist*/
        /*checks it the operand ended and stores the result*/
        if (curr_char == ' ' || curr_char == '\t' || curr_char == ',') {
            *operand_ended = TRUE;
        }/*if an operand was found, updates the operands count and indexes. and "operand_ended" turns to FALSE*/
        if (curr_char != ' ' && curr_char != '\t' && curr_char != ',' &&*operand_ended == TRUE){
            counters->number_of_operands++;
            if (counters->number_of_operands == 1) {
                indexes->first_operand_index = index;
            }
            if (counters->number_of_operands == 2){
                indexes->second_operand_index = index;
            }
            *operand_ended = FALSE;
        }
    }
}
/*checks for the first char in the line.
 * used by the "find_line_components" function and so the flags that are given as arguments
 */
void first_char_check(line_indexes* indexes, char* first_char_found, char curr_char, int index){
    /*if a char was found*/
    if (!(*first_char_found) && curr_char != ' ' && curr_char != '\t') {
        indexes->first_char_index = index;
        *first_char_found = TRUE;
    }
}
/*orders*/
/*
 * checks if an order exists in the current code line. returns TRUE if order was found, else FALSE
 * used by the "define_sentence_type" function.
 */
char is_order(line_counters counters, line_indexes indexes){
    switch (counters.number_of_dots) {/*no dot = no order*/
        case 0:{
            return FALSE;
        }
        /*order condition - dot not between quotation marks. one quote mark is before the dot will produce an error*/
        default:{
            if (counters.number_of_quotation_marks > 1) {
                if (indexes.first_quotation_mark_index < indexes.dot_index &&
                indexes.second_quotation_mark_index > indexes.dot_index) {
                    return FALSE;
                } else {
                    return TRUE;
                }
            }
            return TRUE;
        }
    }
}
/*
 * given a dot index, gets the order after the dot and enters it to the "order" variable in "sentence".
 */
void find_data_order(line* sentence, int dot_index){
    if (dot_index >= 0){
        /*pointer to the line, start from the first char of the order*/
        char* i = sentence->line + dot_index+1;
        int k = 0;
        /*gets the order*/
        while (*i != ' ' && *i != '\t' && *i && k < ORDER_MAX_LENGTH-1) {/*runs until the end of the order*/
            *(sentence->data_parts.order + k) = *i;
            k++;
            i++;
        }
        *(sentence->data_parts.order+k) = '\0';
    }
}
