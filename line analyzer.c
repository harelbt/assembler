#include <stdlib.h>
#include <string.h>
#include "line analyzer.h"
/*--for the first pass--*/
void analyze_sentence(line* sentence, line_indexes* indexes, line_counters* counters) {
    find_line_components(sentence, indexes, counters);/*fills sentence, indexes and counters*/
    define_sentence_type(sentence, *counters, *indexes);/*code or order*/
    prepare_label(sentence, indexes->data_index);
}
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
static int recognize_operator(char* operator, int* opcode, int* function){
    char** endp = NULL;/*for "strtod"*/
    int i = 0;
    char* operators_table[NUMBER_OF_OPERATORS][3] = {{"mov", "0", ""},{"cmp", "1", ""},{"add", "2", "1"},{"sub", "2", "2"},{"lea", "4", ""},
                                    {"clr", "5", "1"},{"not", "5", "2"},{"inc", "5", "3"},{"dec", "5", "4"},{"jmp", "9", "1"},
                                    {"bne","9", "2"},{"jsr", "9", "3"},{"red", "12", ""},{"prn", "13", ""},{"rts", "14", ""}
                                    };
    while (i < NUMBER_OF_OPERATORS){
        if (!strcmp(operator, operators_table[i][0])){/*if operator was found gets the opcode and the function*/
            *opcode = strtod(operators_table[i][1],endp);
            *function = strtod(operators_table[i][2],endp);
            return TRUE;
        }
        i++;
    }
    return FALSE;/*operator wasn't found*/
}
void check_for_operators(line_counters* counters, line_indexes* indexes, line* sentence, int i){
    *sentence->code_parts.operator_name = *(sentence->line + i);
    *(sentence->code_parts.operator_name+1) = *(sentence->line + i + 1);
    *(sentence->code_parts.operator_name+2) = *(sentence->line + i + 2);
    if (i < sentence->length-3){
        *(sentence->code_parts.operator_name+3) = *(sentence->line + i + 3);
        *(sentence->code_parts.operator_name+4) = '\0';
        if (!strcmp(sentence->code_parts.operator_name,"stop") && STOP_OPERATOR_CONDITION){
            sentence->code_parts.opcode = 15;
            counters->number_of_operators++;
            return ;
        } else{
            *(sentence->code_parts.operator_name+3) = '\0';
        }
    }
    if (recognize_operator(sentence->code_parts.operator_name, &sentence->code_parts.opcode, &sentence->code_parts.function) == TRUE
    && OPERATOR_CONDITION){
        counters->number_of_operators++;
        indexes->operator_index = i;
        return ;
    }
}
static void find_line_components(line* sentence, line_indexes* indexes, line_counters* counters) {
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
    /**/
    while (i < sentence->length) {
        curr_char = *(sentence->line + i);
        first_char_check(indexes, &first_char_found, curr_char, i);
        operator_check(sentence, counters, indexes, i);
        operands_check(sentence, counters, indexes, &operand_ended, curr_char, i);
        data_check(indexes, &order_ended, &data_found, dot_found, curr_char, i);
        signs_check(sentence, counters, indexes, curr_char, &colon_found, &semicolon_found, &dot_found,
                    first_char_found, i);
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
void prepare_label(line* sentence, int data_index){
    /*prepares sentence type*/
    if (sentence->flags.is_code == TRUE) {
        sentence->label.extern_or_entry = FALSE;
    } else{
        if (!strcmp(sentence->data_parts.order, "extern")) {
            sentence->label.extern_or_entry = EXTERN;
            strncpy(sentence->label.name, sentence->line + data_index, LABEL_MAX_LENGTH);
        }
    }
}
static void signs_check(line* sentence, line_counters* counters, line_indexes* indexes, char curr_char, char* colon_found,
                        char* semicolon_found, char* dot_found, char first_char_found, int index){
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
        }
    }
}
static void operator_check(line* sentence, line_counters* counters, line_indexes* indexes, int index){
    if (index < sentence->length - 2){
        check_for_operators(counters, indexes, sentence, index);
    }
}
static void data_check(line_indexes* indexes, char* order_ended, char* data_found, char dot_found, char curr_char, int index){
    if (!(*order_ended) && dot_found == TRUE && (curr_char == ' ' || curr_char == '\t')){
        *order_ended = TRUE;
    }
    if (*order_ended == TRUE && dot_found == TRUE && !(*data_found) && curr_char != ' ' && curr_char != '\t'){
        indexes->data_index = index;
        *data_found = TRUE;
    }
}
static void operands_check(line* sentence, line_counters* counters, line_indexes* indexes, char* operand_ended, char curr_char, int index){
    if (sentence->code_parts.opcode != NOT_FOUND) {
        if (curr_char == ' ' || curr_char == '\t') {
            *operand_ended = TRUE;
        }
        if (curr_char != ' ' && curr_char != '\t' && *operand_ended == TRUE){
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
static void first_char_check(line_indexes* indexes, char* first_char_found, char curr_char, int index){
    if (!(*first_char_found) && curr_char != ' ' && curr_char != '\t') {
        indexes->first_char_index = index;
        *first_char_found = TRUE;
    }
}
static void define_sentence_type(line* sentence, line_counters counters, line_indexes indexes) {
    if (counters.number_of_operators > 0) {
        sentence->flags.is_code = TRUE;
    }
    if (is_order(counters, indexes) == TRUE) {
        sentence->flags.is_data = TRUE;
    }
}
static short int is_order(line_counters counters, line_indexes indexes){
    switch (counters.number_of_dots) {
        case 0:{
            return FALSE;
        }
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
void find_data_order(line* sentence, int dot_index){
    if (dot_index >= 0){
        char* i = sentence->line + dot_index+1;
        int k = 0;
        while (*i != ' ' && *i != '\t' && *i && k < ORDER_MAX_LENGTH-1) {
            *(sentence->data_parts.order + k) = *i;
            k++;
            i++;
        }
        *(sentence->data_parts.order+k) = '\0';
    }
}
static void find_label(line* sentence, line_indexes indexes){
    if (indexes.first_quotation_mark_index == NOT_FOUND) {/*a label definition can't be in or after a string*/
        int i = 0;
        /*LABEL_MAX_LENGTH == 32, so max index is 30 and index 31 is '\0' (LABEL_MAX_LENGTH - 2)*/
        while ((i + indexes.first_char_index < indexes.colon_index) && i < LABEL_MAX_LENGTH - 1) {
            *(sentence->label.name + i) = *(sentence->line + (i + indexes.first_char_index));
            i++;
        }
        *(sentence->label.name + i) = '\0';/*end of string*/
    }
}