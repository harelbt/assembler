#include <stdlib.h>
#include <string.h>
#include "parser.h"
void parse_line (line* sentence, error* error_list){
    line_marks_index indexes;
    find_signs(sentence, &indexes, error_list);
    empty_line_check(sentence, indexes);
    if (sentence->flags.is_empty_line == FALSE) {
        comment_check(sentence, error_list, indexes);
        if (sentence->flags.is_comment == FALSE){
            extract_operator(sentence, indexes);
        }
    }
}
static void find_signs(line* sentence, line_marks_index* indexes, error* error_list) {
    int i = 0;
    short int colon_found = 0;
    short int semicolon_found = 0;
    short int dot_found = 0;
    short int first_char_found = 0;
    int number_of_hash_marks = 0;
    int number_of_quotation_marks = 0;
    int number_of_registers = 0;
    assume_no_signes(indexes);
    while (sentence->line[i]) {
        if (!first_char_found && sentence->line[i] != ' ' && sentence->line[i] != '\t') {
            indexes->first_char_index = i;
            first_char_found = 1;
        }
        switch (sentence->line[i]) {
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
static void comment_check(line* sentence, error* error_list, line_marks_index indexes){
    if (indexes.semicolon_index == -1){
        sentence->flags.is_comment = FALSE;
        return;
    }
    if (COMMENT_CONDITION){
        sentence->flags.is_comment = TRUE;
        return;
    }
    if (UNEXPECTED_SEMICOLON_CONDITION){
        report_error(sentence, UNEXPECTED_SEMICOLON);
    }
}
static void empty_line_check (line* sentence, line_marks_index indexes){
    if (indexes.first_char_index == -1){
        sentence->flags.is_empty_line = TRUE;
    } else
        sentence->flags.is_empty_line = FALSE;
}
static void extract_operator(line* sentence, line_marks_index indexes){
    operator_variables op_variables;
    initialize_operator_variables(&op_variables, sentence, indexes);

    if (op_variables.str_length < 3){
        define_as_not_instruction(sentence);
        inspect_non_instruction_line(sentence, indexes);
    }
    find_and_handle_operators(&op_variables);
}

static void find_and_handle_operators(operator_variables* op_variables){
    int i = 0;
    int recognized_opcode = -1;
    int recognized_function = -1;
    while (i < op_variables->str_length - 2){
        strncpy(op_variables->operator, op_variables->sentence->line+i, 3);
        if (recognize_operator(op_variables->operator, &recognized_opcode, &recognized_function) == 1){
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
    char* operators_table[16][3] = {{"mov", "0", ""},{"cmp", "1", ""},{"add", "2", "1"},{"sub", "2", "2"},{"lea", "4", ""},
                                    {"clr", "5", "1"},{"not", "5", "2"},{"inc", "5", "3"},{"dec", "5", "4"},{"jmp", "9", "1"},
                                    {"bne","9", "2"},{"jsr", "9", "3"},{"red", "12", ""},{"prn", "13", ""},{"rts", "14", ""},
                                    {"stop", "15", ""}};
    while (i < 16){
        if (!strcmp(operator, operators_table[i][0])){
            *opcode = strtod(operators_table[i][1],endp);
            *function = strtod(operators_table[i][2],endp);
            return 1;
        }
        i++;
    }
    return 0;
}
static void handle_operators(operator_variables* op_variables, int recognized_opcode, int recognized_function){
    if (!(op_variables->number_of_operators)){
        define_as_not_instruction(op_variables->sentence);
        inspect_non_instruction_line(op_variables->sentence, op_variables->indexes);
    }
    if (op_variables->number_of_operators == 1){
        op_variables->sentence->code_parts.opcode = recognized_opcode;
        op_variables->sentence->code_parts.function = recognized_function;
        op_variables->sentence->flags.is_instruction = 1;
    }
    if (op_variables->number_of_operators > 1){
        report_error(op_variables->sentence, EXTRA_OPERATORS);
    }
}
static void define_as_not_instruction(line* sentence){
    sentence->flags.is_instruction = FALSE;
    sentence->code_parts.opcode = -1;
    sentence->code_parts.function = -1;
    sentence->code_parts.first_operand = NULL;
    sentence->code_parts.second_operand = NULL;
}
void assume_no_signes(line_marks_index* indexes){
    indexes->first_hash_mark_index = -1;
    indexes->first_register_index = -1;
    indexes->semicolon_index = -1;
    indexes->first_char_index = -1;
    indexes->first_quotation_mark = -1;
    indexes->second_quotation_mark = -1;
    indexes->second_register_index = -1;
    indexes->second_hash_mark_index = -1;
    indexes->dot_index = -1;
    indexes->colon_index = -1;
}
void initialize_operator_variables(operator_variables* op_variables, line* sentence, line_marks_index indexes){
    op_variables->number_of_operators = 0;
    op_variables->operator[3] = '\0';
    op_variables->sentence = sentence;
    op_variables->str_length = (int) strlen(sentence->line);
    op_variables->indexes = indexes;
}