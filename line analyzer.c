#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "line analyzer.h"
#include "helpfunctions.h"
void analyze_sentence(line* sentence, line_marks_index* indexes, line_marks_counter* counters) {
    operator op_variables;
    /**/
    initialize_operator_variables(&op_variables);
    assume_no_signs(indexes, counters);
    /**/
    find_line_components(sentence, indexes, counters, &op_variables);
    define_sentence_type(sentence, *counters, *indexes, op_variables);
}
void comment_check(line* sentence, line_marks_index indexes){
    if (indexes.semicolon_index == -1){
        sentence->flags.is_comment = FALSE;
        return;
    }
    if (COMMENT_CONDITION){
        sentence->flags.is_comment = TRUE;
        return;
    }
}
void empty_line_check (line* sentence, line_marks_index indexes){
    if (indexes.first_char_index == -1){
        sentence->flags.is_empty_line = TRUE;
    } else
        sentence->flags.is_empty_line = FALSE;
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
static void define_as_not_code(line* sentence){
    sentence->flags.is_code = FALSE;
    sentence->code_parts.opcode = -1;
    sentence->code_parts.function = -1;
    sentence->code_parts.first_operand = NULL;
    sentence->code_parts.second_operand = NULL;
}
static void define_as_code(line* sentence, int opcode, int function){
    sentence->flags.is_code = TRUE;
    sentence->code_parts.opcode = opcode;
    sentence->code_parts.function = function;
}
void assume_no_signs(line_marks_index* indexes, line_marks_counter* counters){
    indexes->first_hash_mark_index = -1;
    indexes->first_register_index = -1;
    indexes->semicolon_index = -1;
    indexes->first_char_index = -1;
    indexes->first_quotation_mark_index = -1;
    indexes->second_quotation_mark_index = -1;
    indexes->second_register_index = -1;
    indexes->second_hash_mark_index = -1;
    indexes->dot_index = -1;
    indexes->colon_index = -1;
    counters->number_of_hashmarks = 0;
    counters->number_of_registers = 0;
    counters->number_of_colons = 0;
    counters->number_of_dots = 0;
    counters->number_of_quotation_mark = 0;
    counters->number_of_operators = 0;
}
void check_for_operators(operator* op_variables, line_marks_counter* counters, char * line_pointer, int i){
    int str_length = (int) strlen(line_pointer);
    op_variables->operator_name[0] = *(line_pointer+i);
    op_variables->operator_name[1] = *(line_pointer+i+1);
    op_variables->operator_name[2] = *(line_pointer+i+2);
    if (i < str_length-3){
        op_variables->operator_name[3] = *(line_pointer+i+3);
        op_variables->operator_name[4] = '\0';
        if (!strcmp(op_variables->operator_name,"stop") && OPERATOR_CONDITION){
            op_variables->recognized_opcode = 15;
            counters->number_of_operators++;
            return ;
        } else{
            op_variables->operator_name[3] = '\0';
        }
    }
    if (recognize_operator(op_variables->operator_name, &op_variables->recognized_opcode, &op_variables->recognized_function) == 1
    && OPERATOR_CONDITION){
        counters->number_of_operators++;
        return ;
    }
}
void find_line_components(line* sentence, line_marks_index* indexes, line_marks_counter* counters, operator* op_variables){
    int i = 0;
    short int colon_found = 0;
    short int semicolon_found = 0;
    short int dot_found = 0;
    short int first_char_found = 0;
    /**/
    int str_length = (int)strlen(sentence->line);
    char curr_char;
    char* line_pointer = sentence->line;
    /**/
    while (i < str_length) {
        curr_char = *(line_pointer+i);
        if (!first_char_found && curr_char != ' ' && curr_char != '\t') {
            indexes->first_char_index = i;
            first_char_found = 1;
        }
        if (i < str_length - 2){
            check_for_operators(op_variables, counters, line_pointer, i);
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
    find_data_order(sentence, *indexes);
}
void define_sentence_type(line* sentence, line_marks_counter counters, line_marks_index indexes, operator op_variables){
    if (counters.number_of_operators > 0){
        define_as_code(sentence, op_variables.recognized_opcode, op_variables.recognized_function);
    }
    if (counters.number_of_operators == 0){
        define_as_not_code(sentence);
    }
    if (is_order(counters, indexes) == 1){
        define_as_order(sentence);
    } else{
        define_as_not_order(sentence);
    }
}
short int is_order(line_marks_counter counters,line_marks_index indexes){
    switch (counters.number_of_dots) {
        case 0:{
            return 0;
        }
        default:{
            if (counters.number_of_quotation_mark > 1) {
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
void define_as_order(line* sentence){
    sentence->flags.is_data = TRUE;
}
void define_as_not_order(line* sentence){
    sentence->flags.is_data = FALSE;
    sentence->data_parts.data = NULL;
    sentence->data_parts.order[0] = '\0';
}