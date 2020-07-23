#include <stdlib.h>
#include <string.h>
#include "line analyzer.h"
void analyze_sentence(line* sentence, line_marks_index* indexes, line_marks_counter* counters) {
    operator op_variables;
    /**/
    initialize_operator_variables(&op_variables);
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
void check_for_operators(operator* op_variables, line_marks_counter* counters, line_marks_index* indexes, char * line, int i){
    int str_length = (int) strlen(line);
    op_variables->operator_name[0] = *(line + i);
    op_variables->operator_name[1] = *(line + i + 1);
    op_variables->operator_name[2] = *(line + i + 2);
    if (i < str_length-3){
        op_variables->operator_name[3] = *(line + i + 3);
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
        indexes->operator_index = i;
        return ;
    }
}
static void find_line_components(line* sentence, line_marks_index* indexes, line_marks_counter* counters, operator* op_variables){
    int i = 0;
    short int colon_found = 0;
    short int semicolon_found = 0;
    short int dot_found = 0;
    short int first_char_found = 0;
    /**/
    sentence->length = (int)strlen(sentence->line);
    char curr_char = *(sentence->line+i);
    /**/
    while (i < sentence->length) {
        curr_char = *(sentence->line+i);
        if (!first_char_found && curr_char != ' ' && curr_char != '\t') {
            indexes->first_char_index = i;
            first_char_found = 1;
        }
        if (i < sentence->length - 2){
            check_for_operators(op_variables, counters, indexes, sentence->line, i);
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
static void define_sentence_type(line* sentence, line_marks_counter counters, line_marks_index indexes, operator op_variables){
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
static void define_as_order(line* sentence){
    sentence->flags.is_data = TRUE;
}
static void define_as_not_order(line* sentence){
    sentence->flags.is_data = FALSE;
    sentence->data_parts.data = NULL;
    sentence->data_parts.order[0] = '\0';
}
void find_data_order(line* sentence, int dot_index){
    if (dot_index >= 0){
        int i = dot_index;
        int k = 0;
        char curr_char = sentence->line[i];
        int str_length = (int)strlen(sentence->line);
        while (curr_char != ' ' && curr_char != '\t' && i < str_length && k < 6){
            i++;
            curr_char = sentence->line[i];
            if (curr_char != ' ' && curr_char != '\t') {
                sentence->data_parts.order[k] = curr_char;
                k++;
            }
        }
        sentence->data_parts.order[k] = '\0';
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