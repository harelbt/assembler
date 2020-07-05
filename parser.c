#include <string.h>
#include "parser.h"
void parse_line (line* sentence, error* error_list, char* operators_table){
    line_marks_index indexes;
    find_signs(sentence, &indexes, error_list);

    empty_line_check(sentence, indexes);
    if (sentence->flags.is_empty_line == FALSE) {
        comment_check(sentence, error_list, indexes);
        if (sentence->flags.is_comment == FALSE){
            extract_operator(sentence, operators_table, indexes, error_list);
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
                if (!colon_found) {
                    indexes->colon_index = i;
                    colon_found = 1;
                }
                break;
            }
            case ';': {
                if (!semicolon_found) {
                    indexes->semicolon_index = i;
                    semicolon_found = 1;
                }
                break;
            }
            case '.': {
                if (!dot_found) {
                    indexes->dot_index = i;
                    dot_found = 1;
                }
                break;
            }
            case '#': {
                if (number_of_hash_marks < 2) {
                    if (!number_of_hash_marks) {
                        indexes->first_hash_mark_index = i;
                        number_of_hash_marks++;
                        break;
                    }
                    if (number_of_hash_marks == 1) {
                        indexes->second_hash_mark_index = i;
                        number_of_hash_marks++;
                        break;
                    }
                } else {
                    sentence->char_number = i;
                    report_error(sentence, UNEXPECTED_HASHMARK, error_list);
                    break;
                }
            }
            case 'r': {
                int str_length = (int) strlen(sentence->line);
                if (number_of_registers < 2) {
                    if (REGISTER_CONDITION) {
                        if (!number_of_registers) {
                            indexes->first_register_index = i;
                            number_of_registers++;
                            break;
                        }
                        if (number_of_registers == 1) {
                            indexes->second_register_index = i;
                            number_of_registers++;
                        }
                        break;
                    }
                    break;
                } else {
                    sentence->char_number = i;
                    report_error(sentence, UNEXPECTED_REGISTER, error_list);
                    break;
                }
            }
            case '\"': {
                if (number_of_quotation_marks < 2) {
                    if (!number_of_quotation_marks) {
                        indexes->first_quotation_mark = i;
                        number_of_quotation_marks++;
                        break;
                    }
                    if (number_of_quotation_marks == 1) {
                        indexes->second_quotation_mark = i;
                        number_of_quotation_marks++;
                        break;
                    }
                } else {
                    sentence->char_number = i;
                    report_error(sentence, UNEXPECTED_QUOT_MARK, error_list);
                    break;
                }
            }
        }
        i++;
    }
    if (!semicolon_found) {
        indexes->semicolon_index = -1;
    }
    if (!colon_found) {
        indexes->colon_index = -1;
    }
    if (!dot_found) {
        indexes->dot_index = -1;
    }
    if (!first_char_found) {
        indexes->first_char_index = -1;
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
        report_error(sentence, UNEXPECTED_SEMICOLON, error_list);
    }
}
static void empty_line_check (line* sentence, line_marks_index indexes){
    if (indexes.first_char_index == -1){
        sentence->flags.is_empty_line = TRUE;
    } else
        sentence->flags.is_empty_line = FALSE;
}
void extract_operator(line* sentence, char* operators_table, line_marks_index indexes, error* error_list){
    typedef struct {
        int str_length;
        char* operator;
        line sentence;
        int number_of_operators;
        char* operators_table[16][3];
    }operator_variables;
    int number_of_operators = 0;
    char operator [4];
    int str_length = (int) strlen(sentence->line);

    if (str_length < 3){
        define_as_not_instruction(sentence);
        if (indexes.first_register_index >= 0){
            report_error(sentence, REGISTER_NO_OPERATOR, error_list);
        }
        if (indexes.first_hash_mark_index >= 0){
            report_error(sentence, HASHMARK_NO_OPERATOR, error_list);
        }
    }
    operator[3] = '\0';
    find_and_handle_operators(str_length, operator, *sentence, &number_of_operators, operators_table);
}
static int recognize_operator(char* operator, char* operators_table, int* opcode, int* function){
    int i = 0;
    while (i < 16){

        i++;
    }
}
static void find_and_handle_operators(int str_length, char* operator, line sentence, int* number_of_operators, char* operators_table){
    int i = 0;
    int recognized_opcode;
    int recognized_function;
    while (i < str_length - 2){
        strncpy(operator, sentence.line+i, 3);
        if (recognize_operator(operator, operators_table, &recognized_opcode, &recognized_function) == 1){
            if (i == 0 || (i > 0 && (sentence.line[i-1] == ' ' || sentence.line[i-1] == '\t')))
                *number_of_operators++;
        }
        i++;
    }
    handle_operators(number_of_operators, sentence, error_list);
}
static void handle_operators(int number_of_operators, line* sentence, error* error_list){
    if (!number_of_operators){
        define_as_not_instruction(sentence);
    }
    if (number_of_operators == 1){

    }
    if (number_of_operators > 1){
        report_error(sentence, )
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