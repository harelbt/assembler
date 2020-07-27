#include <stdlib.h>
#include <string.h>
#include "line analyzer.h"
void analyze_sentence(line* sentence, line_marks_index* indexes, line_marks_counter* counters) {
    find_line_components(sentence, indexes, counters);
    define_sentence_type(sentence, *counters, *indexes);
}
void empty_or_comment_line_check (line* sentence, line_marks_index indexes){
    /*empty line check*/
    if (indexes.first_char_index == -1) {
        sentence->flags.is_empty_line = TRUE;
    }
    /*comment check*/
    if (COMMENT_CONDITION){
        sentence->flags.is_comment = TRUE;
        return;
    }
}
static int recognize_operator(char* operator, int* opcode, int* function){
    char** endp = NULL;
    int i = 0;
    char* operators_table[15][3] = {{"mov", "0", ""},{"cmp", "1", ""},{"add", "2", "1"},{"sub", "2", "2"},{"lea", "4", ""},
                                    {"clr", "5", "1"},{"not", "5", "2"},{"inc", "5", "3"},{"dec", "5", "4"},{"jmp", "9", "1"},
                                    {"bne","9", "2"},{"jsr", "9", "3"},{"red", "12", ""},{"prn", "13", ""},{"rts", "14", ""},
                                    };
    while (i < 15){
        if (!strcmp(operator, operators_table[i][0])){
            *opcode = strtod(operators_table[i][1],endp);
            *function = strtod(operators_table[i][2],endp);
            return 1;
        }
        i++;
    }
    return 0;
}
void check_for_operators(line_marks_counter* counters, line_marks_index* indexes, line* sentence, int i){
    *sentence->code_parts.operator_parts.operator_name = *(sentence->line + i);
    *(sentence->code_parts.operator_parts.operator_name+1) = *(sentence->line + i + 1);
    *(sentence->code_parts.operator_parts.operator_name+2) = *(sentence->line + i + 2);
    if (i < sentence->length-3){
        *(sentence->code_parts.operator_parts.operator_name+3) = *(sentence->line + i + 3);
        *(sentence->code_parts.operator_parts.operator_name+4) = '\0';
        if (!strcmp(sentence->code_parts.operator_parts.operator_name,"stop") && OPERATOR_CONDITION){
            sentence->code_parts.operator_parts.opcode = 15;
            counters->number_of_operators++;
            return ;
        } else{
            sentence->code_parts.operator_parts.operator_name[3] = '\0';
        }
    }
    if (recognize_operator(sentence->code_parts.operator_parts.operator_name, &sentence->code_parts.operator_parts.opcode, &sentence->code_parts.operator_parts.function) == 1
    && OPERATOR_CONDITION){
        counters->number_of_operators++;
        indexes->operator_index = i;
        return ;
    }
}
static void find_line_components(line* sentence, line_marks_index* indexes, line_marks_counter* counters){
    int i = 0;
    short int colon_found = 0;
    short int semicolon_found = 0;
    short int dot_found = 0;
    short int first_char_found = 0;
    short int operand_ended = 0;
    short int order_ended = 0;
    int data_found = 0;
    /**/
    char curr_char;
    /**/
    while (i < sentence->length) {
        curr_char = *(sentence->line+i);
        if (!first_char_found && curr_char != ' ' && curr_char != '\t') {
            indexes->first_char_index = i;
            first_char_found = 1;
        }
        if (i < sentence->length - 2){
            check_for_operators(counters, indexes, sentence, i);
        }
        if (sentence->code_parts.operator_parts.opcode != -1) {
            if (curr_char == ' ' || curr_char == '\t') {
                operand_ended = 1;
            }
            if (curr_char != ' ' && curr_char != '\t' && operand_ended == 1){
                counters->number_of_operands++;
                if (counters->number_of_operands == 1) {
                    indexes->first_label_operator_index = i;
                }
                if (counters->number_of_operators == 2){
                    indexes->second_label_operator_index = i;
                }
                operand_ended = 0;
            }
        }
        if (!order_ended && dot_found == 1 && (curr_char == ' ' || curr_char == '\t')){
            order_ended = 1;
        }
        if (order_ended == 1 && dot_found == 1 && !data_found && curr_char != ' ' && curr_char != '\t'){
            indexes->data_index = i;
            data_found = 1;
        }
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
            case '#': {
                HASHMARK_CASE

            }
            case 'r': {
                REGISTER_CASE
            }
            case '\"': {
                QUOT_MARK_CASE
            }
        }
        i++;
    }
}
static void define_sentence_type(line* sentence, line_marks_counter counters, line_marks_index indexes) {
    if (counters.number_of_operators > 0) {
        sentence->flags.is_code = TRUE;
    }
    if (is_order(counters, indexes) == 1) {
        sentence->flags.is_data = TRUE;
    }
}
static short int is_order(line_marks_counter counters,line_marks_index indexes){
    switch (counters.number_of_dots) {
        case 0:{
            return 0;
        }
        default:{
            if (counters.number_of_quotation_marks > 1) {
                if (indexes.first_quotation_mark_index < indexes.dot_index &&
                indexes.second_quotation_mark_index > indexes.dot_index) {
                    return 0;
                } else {
                    return 1;
                }
            }
            return 1;
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
static void find_label(line* sentence, line_marks_index indexes){
    if (indexes.first_quotation_mark_index == -1) {/*a label definition can't be in or after a string*/
        int i = 0;
        /*LABEL_MAX_LENGTH == 32, so max index is 30 and index 31 is '\0' (LABEL_MAX_LENGTH - 2)*/
        while ((i + indexes.first_char_index < indexes.colon_index) && i < LABEL_MAX_LENGTH - 1) {
            *(sentence->label.name + i) = *(sentence->line + (i + indexes.first_char_index));
            i++;
        }
        *(sentence->label.name + i) = '\0';/*end of string*/
    }
}