#include <stdlib.h>
#include <string.h>
#include "parser.h"
void parse_line (line* sentence){
    line_marks_index indexes;
    line_marks_counter counters;
    analize_sentence(sentence, &indexes, &counters);
    empty_line_check(sentence, indexes);
    if (sentence->flags.is_empty_line == FALSE) {
        comment_check(sentence, indexes);
        if (sentence->flags.is_comment == FALSE){
            build_sentence();
        }
    }
}
static void analize_sentence(line* sentence, line_marks_index* indexes, line_marks_counter* counters) {
    int i = 0;
    int str_length = (int) strlen(sentence->line);
    /**/
    short int colon_found = 0;
    short int semicolon_found = 0;
    short int dot_found = 0;
    short int first_char_found = 0;
    /**/
    operator op_variables;
    char* line_pointer = sentence->line;
    char curr_char;
    /**/
    initialize_operator_variables(&op_variables, sentence, *indexes);
    assume_no_signes(indexes, counters);
    while (i < str_length) {
        curr_char = sentence->line[i];
        if (!first_char_found && curr_char != ' ' && curr_char != '\t') {
            indexes->first_char_index = i;
            first_char_found = 1;
        }
        if (i < str_length - 2){
            check_for_operators(&op_variables, counters, line_pointer, i);
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
    if (counters->number_of_operators == 1){
        define_as_instruction(sentence,recognized_opcode, recognized_function);
    }
    if (counters->number_of_operators > 1){
        define_as_not_instruction(sentence);
    }
}
static void comment_check(line* sentence, line_marks_index indexes){
    if (indexes.semicolon_index == -1){
        sentence->flags.is_comment = FALSE;
        return;
    }
    if (COMMENT_CONDITION){
        sentence->flags.is_comment = TRUE;
        return;
    }
}
static void empty_line_check (line* sentence, line_marks_index indexes){
    if (indexes.first_char_index == -1){
        sentence->flags.is_empty_line = TRUE;
    } else
        sentence->flags.is_empty_line = FALSE;
}
static void extract_operator(line* sentence, line_marks_index indexes){
    operator op_variables;
    initialize_operator_variables(&op_variables, sentence, indexes);
    if (op_variables.str_length < 3){
        define_as_not_instruction(sentence);
    }
    find_and_handle_operators(&op_variables);
}

static void find_and_handle_operators(operator* op_variables){
    int i = 0;
    int recognized_opcode = -1;
    int recognized_function = -1;
    /**/
    while (i < op_variables->str_length - 2){
        strncpy(op_variables->operator_name, op_variables->sentence->line + i, 3);
        if (recognize_operator(op_variables->operator_name, &recognized_opcode, &recognized_function) == 1){
            if (i == 0 || (i > 0 && (op_variables->sentence->line[i-1] == ' ' || op_variables->sentence->line[i-1] == '\t')))
                op_variables->number_of_operators++;
        }
        i++;
    }
    handle_operators(op_variables, recognized_opcode, recognized_function);
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
static void handle_operators(operator* op_variables, int recognized_opcode, int recognized_function){
    if (!(op_variables->number_of_operators)){
        define_as_not_instruction(op_variables->sentence);
    }
    if (op_variables->number_of_operators == 1){
        op_variables->sentence->code_parts.opcode = recognized_opcode;
        op_variables->sentence->code_parts.function = recognized_function;
        op_variables->sentence->flags.is_instruction = 1;
    }
}
static void define_as_not_instruction(line* sentence){
    sentence->flags.is_instruction = FALSE;
    sentence->code_parts.opcode = -1;
    sentence->code_parts.function = -1;
    sentence->code_parts.first_operand = NULL;
    sentence->code_parts.second_operand = NULL;
}
static void define_as_instruction(line* sentence, int opcode, int function){
    sentence->flags.is_instruction = TRUE;
    sentence->code_parts.opcode = opcode;
    sentence->code_parts.function = function;
}
void assume_no_signes(line_marks_index* indexes, line_marks_counter* counters){
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
void initialize_operator_variables(operator* op_variables, line* sentence, line_marks_index indexes){
    op_variables->number_of_operators = 0;
    op_variables->recognized_function = -1;
    op_variables->recognized_opcode = -1;
}
void check_for_operators(operator* op_variables, line_marks_counter* counters, const char* line_pointer, int i){
    int str_length = (int) strlen(line_pointer);
    op_variables->operator_name[0] = *(line_pointer+i);
    op_variables->operator_name[1] = *(line_pointer+i+1);
    op_variables->operator_name[2] = *(line_pointer+i+2);
    if (i < str_length-3){
        op_variables->operator_name[3] = *(line_pointer+i+3);
        op_variables->operator_name[4] = '\0';
        if (!strcmp(op_variables->operator_name,"stop") && (i == 0 || line_pointer[i-1] == ' ' || line_pointer[i-1] == '\t')
        && (i == str_length-1 || line_pointer[i+1] == ' ' || line_pointer[i+1] == '\t')){
            op_variables->recognized_opcode = 15;
            counters->number_of_operators++;
            return;
        } else{
            op_variables->operator_name[3] = '\0';
        }
    }
    if (recognize_operator(op_variables->operator_name, &op_variables->recognized_opcode, &op_variables->recognized_function) == 1){
        counters->number_of_operators++;
    }
}