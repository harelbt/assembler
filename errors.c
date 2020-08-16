#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "errors.h"
#include "in out tools.h"
/*~~general functions~~*/
char errors_inspection(line* sentence, line_indexes* indexes, line_counters* counters){
    char error_found = FALSE;
    if (*(sentence->label.name) != '\0' && sentence->label.extern_or_entry == FALSE){
        inspect_label(sentence, indexes, counters, indexes->first_char_index, &error_found);
    }
    if (*(sentence->label.name) == '\0' && indexes->colon_index != NOT_FOUND &&
            ((indexes->first_quotation_mark_index != NOT_FOUND && indexes->colon_index < indexes->first_quotation_mark_index) ||
            indexes->first_quotation_mark_index == NOT_FOUND)){
        report_error(sentence->line, MISSING_LABEL, counters, indexes->colon_index);
        error_found = TRUE;
    }
    if (sentence->flags.is_data == TRUE && sentence->flags.is_code == TRUE){
        report_error(sentence->line, MIXED_SENTENCE, counters);
        error_found = TRUE;
    } else if (sentence->flags.is_data == TRUE){
        error_found = inspect_order_line(sentence, indexes, counters);
    } else if(sentence->flags.is_code == TRUE){
        inspect_code_line(sentence, indexes, counters, &error_found);
    } if (sentence->flags.is_data == FALSE && sentence->flags.is_code == FALSE){
        report_error(sentence->line, NO_SENTENCE_TYPE, counters);
        error_found = TRUE;
    }
    return error_found;
}
void report_error(char* line, char error_code, line_counters* counters, ...){
    int unexpected;
    short int print_char_indication = FALSE;
    counters->error_number++;
    if (counters->error_number == 1){
        printf("*************************************************************************************\n"
               "ERRORS LIST:\n\n");
    }
    printf("%d) ", counters->error_number);
    printf("ERROR: ");
    switch (error_code) {
        case UNEXPECTED_CHARACTER:{
            int unexpected_index;
            va_list argp;
            va_start(argp, counters);
            unexpected = va_arg(argp, int);
            unexpected_index = va_arg(argp, int);
            va_end(argp);
            printf("%c is an unexpected character. line %d.\n", unexpected, counters->line_number);
            puts(line);
            print_visual_indication(unexpected_index, line);
            puts("________________________________________________________________________________________________________");
            return;
        }
        case NO_SOURCE_REGISTER_ALLOWED:{
            print_char_indication = TRUE;
            printf("The source operand for this operator can't be a register. line %d.\n", counters->line_number);
            break;
        }
        case NO_DEST_REGISTER_ALLOWED:{
            print_char_indication = TRUE;
            printf("The destination operand for this operator can't be a register. line %d.\n", counters->line_number);
            break;
        }
        case EXTRA_OPERATORS:{
            printf("Extra operator in line %d.\n", counters->line_number);
            break;
        }
        case ZERO_OPERANDS:{
            printf("No operands found in line %d.\n", counters->line_number);
            break;
        }
        case EXTRA_OPERANDS:{
            printf("Too MANY operands or components found in line, check operator's requirement. line %d.\n", counters->line_number);
            break;
        }
        case FEW_OPERANDS:{
            printf("Too FEW operands found in line, check operator's requirement. line %d.\n", counters->line_number);
            break;
        }
        case NO_SOURCE_JUMP_LABEL_ALLOWED:{
            print_char_indication = TRUE;
            printf("The source operand can't be a \"jump to label\" operand. line %d.\n", counters->line_number);
            break;
        }
        case NO_DEST_JUMP_LABEL_ALLOWED:{
            print_char_indication = TRUE;
            printf("The destination operand for this operator can't be a \"jump to label\" operand. line %d.\n", counters->line_number);
            break;
        }
        case NO_DEST_NUMBER_ALLOWED:{
            print_char_indication = TRUE;
            printf("The destination operand for this operator can't be a number. line %d.\n", counters->line_number);
            break;
        }
        case NO_SOURCE_NUMBER_ALLOWED:{
            print_char_indication = TRUE;
            printf("The source operand for this operator can't be a number. line %d.\n", counters->line_number);
            break;
        }
        case NOT_AN_OPERAND:{
            print_char_indication = TRUE;
            printf("Illegal operand in line %d.\n", counters->line_number);
            break;
        }
        case ILLEGAL_CODE_BEFORE_OPERATOR:{
            print_char_indication = TRUE;
            printf("Illegal code before the operator. line %d.\n", counters->line_number);
            break;
        }
        case MIXED_SENTENCE:{
            printf("Mixed code and data order in line %d.\n", counters->line_number);
            break;
        }
        case NO_SENTENCE_TYPE:{
            printf("Sentence is not code or data order in line %d. check the syntax.\n", counters->line_number);
            break;
        }
        case SINGLE_QUOTMARK:{
            print_char_indication = TRUE;
            printf("Opening quote mark without closing quote mark in line %d.\n", counters->line_number);
            break;
        }
        case STRING_NO_ORDER:{
            printf("String without a .string order before in line %d.\n", counters->line_number);
            break;
        }
        case CODE_AFTER_QUOTE:{
            print_char_indication = TRUE;
            printf("Code was found after a string in line %d.\n", counters->line_number);
            break;
        }
        case ORDER_NO_STRING:{
            printf(".string order without a string afterwards. line %d.\n", counters->line_number);
            break;
        }
        case CHARS_BETWEEN_ORDER_STRING:{
            print_char_indication = TRUE;
            printf("Unexpected code between the .string order and the string afterwards. line %d.\n", counters->line_number);
            break;
        }
        case NO_SPACE_BEFORE_QUOTE:{
            print_char_indication = TRUE;
            printf("A string needs to be written after a space or spaces. line %d.\n", counters->line_number);
            break;
        }
        case CHARS_BEFORE_STRING_ORDER:{
            print_char_indication = TRUE;
            printf("Unexpected code before the .string order in line %d.\n", counters->line_number);
            break;
        }
        case DATA_NO_ORDER:{
            printf("Data supplied without .data order in line %d.\n", counters->line_number);
            break;
        }
        case ORDER_NO_DATA:{
            printf(".Data order supplied without data in line %d.\n", counters->line_number);
            break;
        }
        case CHARS_BEFORE_DATA_ORDER:{
            print_char_indication = TRUE;
            printf("Unexpected code before .data order in line %d.\n", counters->line_number);
            break;
        }
        case NO_COMMA_BETWEEN:{
            print_char_indication = TRUE;
            printf("No comma is placed between numbers in line %d.\n", counters->line_number);
            break;
        }
        case COMMA_NO_NUMBERS:{
            print_char_indication = TRUE;
            printf("Data can't begin with ',' without a number before. line %d.\n", counters->line_number);
            break;
        }
        case COMMA_NO_FOLLOWING_NUMBER:{
            print_char_indication = TRUE;
            printf("No number was found after ','. line %d.\n", counters->line_number);
            break;
        }
        case NO_NUMBERS_BETWEEN_COMMAS:{
            print_char_indication = TRUE;
            printf("No numbers between two commas in line %d.\n", counters->line_number);
            break;
        }
        case ARITHMETIC_SIGN_NOT_IN_PLACE:{
            print_char_indication = TRUE;
            printf("Arithmetic sign can only be after ',' or spaces. line %d.\n", counters->line_number);
            break;
        }
        case SIGN_NOT_NY_NUM:{
            print_char_indication = TRUE;
            printf("Arithmetic sign is not by a number. line %d.\n", counters->line_number);
            break;
        }
        case NO_SUCH_ORDER:{
            print_char_indication = TRUE;
            printf("Illegal order in line %d.\n", counters->line_number);
            break;
        }
        case STRING_BEFORE_STRING_ORDER:{
            print_char_indication = TRUE;
            printf("String before .string order. line %d.\n", counters->line_number);
            break;
        }
        case CHARS_BEFORE_EXTERN_OR_ENTRY:{
            print_char_indication = TRUE;
            printf("Unexpected code before .extern or .entry order in line %d.\n", counters->line_number);
            break;
        }
        case EXTERN_ENTRY_NO_LABEL:{
            print_char_indication = TRUE;
            printf(".extern or .entry order but no label was found. line %d.\n", counters->line_number);
            break;
        }
        case LABEL_TOO_LONG:{
            print_char_indication = TRUE;
            printf("The LABEL is TOO LONG. _label should be in length of 1-31_. line %d.\n", counters->line_number);
            break;
        }
        case MISSING_LABEL:{
            print_char_indication = TRUE;
            printf("The LABEL is MISSING. _label should be in length of 1-31_. line %d.\n", counters->line_number);
            break;
        }
        case LABEL_STARTS_WITH_NUMBER:{
            print_char_indication = TRUE;
            printf("A label can begin only with a non number character. line %d.\n", counters->line_number);
            break;
        }
        case LABEL_NOT_BY_COLON:{
            print_char_indication = TRUE;
            printf("The label is not by the colon. line %d.\n", counters->line_number);
            break;
        }
        case SPACE_IN_LABEL:{
            print_char_indication = TRUE;
            printf("White chars are not allowed in a label or before a colon. line %d.\n", counters->line_number);
            break;
        }
        case LABEL_DOESNT_START_WITH_LETTER:{
            print_char_indication = TRUE;
            printf("A label can begin only with a letter. line %d.\n", counters->line_number);
            break;
        }
        case ILLEGAL_LABEL_SYNTAX:{
            print_char_indication = TRUE;
            printf("Illegal character in label. line %d.\n", counters->line_number);
            break;
        }
        case LABEL_IS_RESERVED:{
            printf("Labels can't be a reserved word. line %d.\n", counters->line_number);
            break;
        }
        case SECOND_LABEL_DEFINITION:{
            printf("Labels can't be defined more than once. line %d.\n", counters->line_number);
            break;
        }
        case ENTRY_NOT_EXIST:{
            printf("Entry order was given, but the label supplied isn't define. line %d.\n", counters->line_number);
            break;
        }
        case NUMBER_TOO_LARGE:{
            print_char_indication = 1;
            printf("Number too large. line %d.\n", counters->line_number);
            break;
        }
        default:
            return;
    }
    PRINT_ERROR_DESCRIPTION
}
/*~~order inspection section~~*/
static char inspect_order_line(line* sentence, line_indexes* indexes, line_counters* counters){
    char error_found = FALSE;
    if (!is_order_proper(sentence)) {
        report_error(sentence->line, NO_SUCH_ORDER, counters, indexes->dot_index);
        error_found = FALSE;
        return error_found;
    }
    detect_string_errors(sentence, indexes, counters, &error_found);
    detect_data_errors(sentence, indexes, counters, &error_found);
    detect_extern_entry_errors(sentence, indexes, counters, &error_found);
    return error_found;
}
static short int is_order_proper(line* sentence) {
    short int is_proper = TRUE;
    if (!strcmp(sentence->data_parts.order, "data") || !strcmp(sentence->data_parts.order, "string") ||
        !strcmp(sentence->data_parts.order, "entry") || !strcmp(sentence->data_parts.order, "extern") ||
            !strcmp(sentence->data_parts.order, "stop")) {
        return is_proper;
    }
    is_proper = FALSE;
    return is_proper;
}
static void detect_string_errors(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found) {
    short int is_string_order;
    /**/
    if (!strcmp(sentence->data_parts.order, "string")){
        is_string_order = TRUE;
    } else{
        is_string_order = FALSE;
    }
    /**/
    if (counters->number_of_quotation_marks == 0){
        if (is_string_order == TRUE){
            report_error(sentence->line, ORDER_NO_STRING, counters);
            *error_found = TRUE;
        }
    }
    /**/
    if (counters->number_of_quotation_marks == 1) {
        report_error(sentence->line, SINGLE_QUOTMARK, counters, indexes->first_quotation_mark_index);
        *error_found = TRUE;
    }
    /**/
    if (counters->number_of_quotation_marks >= 2) {
        detect_two_quotes_errors(sentence, counters, indexes, is_string_order, error_found);
    }
    if (is_string_order == TRUE) {
        check_pre_order_chars(sentence, indexes, counters, error_found);
    }
}
static void detect_two_quotes_errors(line* sentence, line_counters* counters, line_indexes* indexes, short int is_string_order, char* error_found){
    int char_after_string_index = find_next_word(sentence->line, indexes->second_quotation_mark_index+1);
    int char_after_order_index = find_next_word(sentence->line, indexes->dot_index+(int)strlen(sentence->data_parts.order)+1);
    if (is_string_order == FALSE) {
        report_error(sentence->line, STRING_NO_ORDER, counters);
        *error_found = TRUE;
    }
    /*if code was found after the string, ignores the next char is in the '\0' index (the string's length)*/
    if (indexes->second_quotation_mark_index < char_after_string_index && char_after_string_index < sentence->length){
        report_error(sentence->line, CODE_AFTER_QUOTE, counters, char_after_string_index);
        *error_found = TRUE;
    }
    if (is_string_order == TRUE){
        if (indexes->first_quotation_mark_index < indexes->dot_index){
            report_error(sentence->line, STRING_BEFORE_STRING_ORDER, counters, indexes->second_quotation_mark_index);
            *error_found = TRUE;
        }
        if (char_after_order_index < indexes->first_quotation_mark_index){
            report_error(sentence->line, CHARS_BETWEEN_ORDER_STRING, counters, char_after_order_index);
            *error_found = TRUE;
        }
        if (indexes->first_quotation_mark_index > 0 && *(sentence->line + indexes->first_quotation_mark_index-1) != ' ' &&
                indexes->first_quotation_mark_index > 0 && *(sentence->line + indexes->first_quotation_mark_index-1) != '\t'){
            report_error(sentence->line, NO_SPACE_BEFORE_QUOTE, counters, indexes->first_quotation_mark_index);
            *error_found = TRUE;
        }
    }
}
static void detect_data_errors(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found){
    short int is_data_order;
    if(!strcmp(sentence->data_parts.order,"data")){
        is_data_order = TRUE;
    } else{
        is_data_order = FALSE;
    }
    if (counters->number_of_quotation_marks == 0){
        if (is_data_order == TRUE){
            if (!is_data_values_proper(sentence, indexes, counters)){
                *error_found = TRUE;
            }
            /*unexpected chars check*/
            check_pre_order_chars(sentence, indexes, counters, error_found);
        }
        if (is_data_order == FALSE){
            if (is_data_values_proper(sentence, indexes, counters) == TRUE){
                report_error(sentence->line, DATA_NO_ORDER, counters);
                *error_found = TRUE;
            }
        }
    }
}
static short int is_data_values_proper(line* sentence, line_indexes* indexes, line_counters* counters){
    short int is_data_values_proper = TRUE;
    short int is_data_order;
    if(!strcmp(sentence->data_parts.order,"data")){
        is_data_order = TRUE;
    } else{
        is_data_order = FALSE;
    }
    if (indexes->data_index == NOT_FOUND){/*no data was found*/
        if (is_data_order == TRUE) {
            report_error(sentence->line, ORDER_NO_DATA, counters);
        }
        is_data_values_proper = FALSE;
    }else {
        if (inspect_data_values(sentence, indexes->data_index, counters) == TRUE) {
            is_data_values_proper = FALSE;
        }
    }
    return is_data_values_proper;
}
static short int inspect_data_values(line* sentence, int index, line_counters* counters) {
    char error_found = FALSE;
    char last_char = '\0';
    char curr_char = *(sentence->line + index);
    short int is_after_comma = FALSE;
    short int did_number_appeared = FALSE;
    short int is_data_order;
    if (!strcmp(sentence->data_parts.order, "data")) {
        is_data_order = TRUE;
    } else {
        is_data_order = FALSE;
    }
    while (index < sentence->length) {
        /*checks if current char is illegal*/
        if (ILLEGAL_DATA_CHAR_CONDITION) {
            if (is_data_order == TRUE) {
                report_error(sentence->line, UNEXPECTED_CHARACTER, counters, curr_char, index);
            }
            error_found = TRUE;/*indicates that in general, the values are not proper*/
            update_loop_data_inspection_variables(sentence, &curr_char, &last_char, &index);
        } else {/*if current char is legal*/
            values_check(sentence, counters, curr_char, last_char, is_after_comma, is_data_order, &error_found,
                         did_number_appeared, index);
            check_if_after_comma(&curr_char, &is_after_comma);
            check_number_appearance(&did_number_appeared, curr_char);
            update_loop_data_inspection_variables(sentence, &curr_char, &last_char, &index);
        }
    }
        return error_found;
}
static void values_check(line* sentence, line_counters* counters, char curr_char, int last_char, short int is_after_comma,
                         short int is_data_order, char* error_found, short int did_number_appeared, int index) {
        if (is_after_comma == TRUE && curr_char == ',') {
            if (is_data_order == TRUE) {
                report_error(sentence->line, NO_NUMBERS_BETWEEN_COMMAS, counters, index);
            }
            *error_found = TRUE;
        }
        if (curr_char == ',' && did_number_appeared == FALSE) {
            if (is_data_order == TRUE) {
                report_error(sentence->line, COMMA_NO_NUMBERS, counters, index-1);
            }
            *error_found = TRUE;
        }
        if ((curr_char == '-' || curr_char == '+') && index + 1 != sentence->length && *(sentence->line + index) < '0'
        && *(sentence->line + index) > '9'){
            report_error(sentence->line, SIGN_NOT_NY_NUM, counters, index);
            *error_found = TRUE;
        }
        if (did_number_appeared == TRUE) {
            if ((last_char == ' ' || last_char == '\t') && curr_char != ' ' && curr_char != '\t' &&
                is_after_comma == FALSE && curr_char != ',') {/*if no comma was found before a number*/
                if (is_data_order == TRUE) {
                    report_error(sentence->line, NO_COMMA_BETWEEN, counters, index);
                }
                *error_found = TRUE;
            }
            if (index == sentence->length-1 && curr_char == ','){
                report_error(sentence->line, COMMA_NO_FOLLOWING_NUMBER, counters, index);
                *error_found = TRUE;
            }
        }
        if ((curr_char == '-' || curr_char == '+') && *(sentence->line + index - 1) != ' ' &&
                *(sentence->line + index - 1) != '\t' && *(sentence->line + index - 1) != ','){
            report_error(sentence->line, ARITHMETIC_SIGN_NOT_IN_PLACE, counters, index);
            *error_found = TRUE;
        }
    }
static void check_if_after_comma(const char *curr_char, short int *is_after_comma) {
    if (*curr_char == ',') {
        *is_after_comma = TRUE;
    } else if (*curr_char != ' ' && *curr_char != '\t') {
        *is_after_comma = FALSE;
    }
}
static void update_loop_data_inspection_variables(line* sentence, char* curr_char, char* last_char, int* index){
    if (*index < sentence->length) {
        (*index)++;
        *last_char = *curr_char;
        *curr_char = *(sentence->line + (*index));
    }
}
static void detect_extern_entry_errors(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found) {
    if (!strcmp(sentence->data_parts.order, "entry") || !strcmp(sentence->data_parts.order, "extern")) {
        inspect_data_label(sentence, indexes, counters, error_found);
        check_pre_order_chars(sentence, indexes, counters, error_found);
        check_after_data_chars(sentence, counters, indexes, error_found);
    }
}
static void inspect_data_label(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found){
    int order_end_index = indexes->dot_index + (int) strlen(sentence->data_parts.order);
    if (indexes->data_index == NOT_FOUND) {/*label wasn't found*/
        report_error(sentence->line, EXTERN_ENTRY_NO_LABEL, counters, order_end_index);
        *error_found = TRUE;
    } else {
        inspect_label(sentence, indexes, counters, indexes->data_index, error_found);
    }
}
static void check_after_data_chars(line* sentence, line_counters* counters, line_indexes* indexes, char* error_found){
    short int data_ended = FALSE;
    char* i = sentence->line + indexes->data_index;
    while (*i) {
        if (!data_ended && (*i == ' ' || *i == '\t')){
            data_ended = TRUE;
        }
        if (data_ended == TRUE && *i != ' ' && *i != '\t') {
            report_error(sentence->line, UNEXPECTED_CHARACTER, counters, *i, i - sentence->line);
            *error_found = TRUE;
        }
        i++;
    }
}
static void check_pre_order_chars(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found){
    char error_code;
    if (!strcmp(sentence->data_parts.order, "data")){
        error_code = CHARS_BEFORE_DATA_ORDER;
    }
    if (!strcmp(sentence->data_parts.order, "string")){
        error_code = CHARS_BEFORE_STRING_ORDER;
    }
    if (!strcmp(sentence->data_parts.order, "extern") || !strcmp(sentence->data_parts.order, "entry")){
        error_code = CHARS_BEFORE_EXTERN_OR_ENTRY;
    }
    /*we are  here only if the first dot is a proper order, so the dot index surely is the index of the order*/
    /*check if there are chars between the label and the order*/
    if (*(sentence->label.name) != '\0' && indexes->colon_index < indexes->dot_index){
        int index_of_word_after_label = find_next_word(sentence->line, indexes->colon_index+1);
        if (*(sentence->line + index_of_word_after_label) != '.') {
            report_error(sentence->line, error_code, counters, index_of_word_after_label);
            *error_found = TRUE;
        }
    } else if (*(sentence->line + indexes->first_char_index) != '.'){
        report_error(sentence->line, error_code, counters, indexes->first_char_index);
        *error_found = TRUE;
    }
}
/*~~code inspection section~~*/
static void inspect_code_line(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found) {
    /*the functions below report errors and return 1 if error found*/
    short int is_operand_count_not_okay = check_operands_count(sentence, indexes, counters, error_found);
    short int is_operators_count_not_okay = check_operators_count(sentence, indexes, counters, error_found);
    if (!is_operand_count_not_okay && !is_operators_count_not_okay) {
        check_operands_syntax(sentence, indexes, counters, error_found);
        check_pre_operator_chars(sentence, indexes, counters, error_found);
    }
}
static short int check_operands_count(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found){
    short int is_error = FALSE;
    switch (sentence->code_parts.opcode) {
        case 0:{}
        case 1:{}
        case 2:{}
        case 3:{}
        case 4:{
            if (counters->number_of_operands < 2){
                report_error(sentence->line, FEW_OPERANDS, counters);
                *error_found = TRUE;
                is_error = TRUE;
            }
            if (counters->number_of_operands > 2){
                report_error(sentence->line, EXTRA_OPERANDS, counters);
                *error_found = TRUE;
                is_error = TRUE;
            }
            break;
        }
        case 5:{}
        case 6:{}
        case 7:{}
        case 8:{}
        case 9:{}
        case 10:{}
        case 11:{}
        case 12:{}
        case 13:{
            if (counters->number_of_operands < 1){
                report_error(sentence->line, FEW_OPERANDS, counters);
                *error_found = TRUE;
                is_error = TRUE;
            }
            if (counters->number_of_operands > 1){
                report_error(sentence->line, EXTRA_OPERANDS, counters);
                *error_found = TRUE;
                is_error = TRUE;
            }
            break;
        }
        case 14:{}
        case 15:{
            if (counters->number_of_operands > 0){
                report_error(sentence->line, EXTRA_OPERANDS, counters);
                *error_found = TRUE;
                is_error = TRUE;
            }
            break;
        }
    }
    return is_error;
}
static short int check_operators_count(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found){
    short int is_error = FALSE;
    if (counters->number_of_operators > 1){
        report_error(sentence->line, EXTRA_OPERATORS, counters);
        *error_found = TRUE;
        is_error = TRUE;
    }
    return is_error;
}
static void check_operands_syntax(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found){
    switch (sentence->code_parts.opcode) {
        case 0:{}
        case 1:{}
        case 2:{}
        case 3:{}
        case 4:{
            check_source_operand_syntax(sentence, indexes, counters, error_found);
            check_dest_operand_syntax(sentence, indexes, counters, error_found);
            break;
        }
        case 5:{}
        case 6:{}
        case 7:{}
        case 8:{}
        case 9:{}
        case 10:{}
        case 11:{}
        case 12:{}
        case 13:{
            check_dest_operand_syntax(sentence, indexes, counters, error_found);
            break;
        }
        default:{
            return;
        }
    }
}
static void check_source_operand_syntax(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found){
    if (is_operand_proper(sentence, indexes, SOURCE_OPERAND) == TRUE) {
        if (*(sentence->line + indexes->first_operand_index) == '#'){
            check_number_size(sentence->line, indexes->first_operand_index, counters, error_found);
        }
        /*"jump to label" is not allowed as a source operand*/
        if (*(sentence->line+indexes->first_operand_index) == '&'){
            report_error(sentence->line, NO_SOURCE_JUMP_LABEL_ALLOWED, counters, indexes->first_operand_index);
            *error_found = TRUE;
        }
        /*opcode 4 has more restrictions*/
        if (sentence->code_parts.opcode == 4){
            if (*(sentence->line + indexes->first_operand_index) == '#') {
                report_error(sentence->line, NO_SOURCE_NUMBER_ALLOWED, counters, indexes->first_operand_index);
                *error_found = TRUE;
            }
            if (indexes->first_operand_index == indexes->first_register_index){
                report_error(sentence->line, NO_SOURCE_REGISTER_ALLOWED, counters, indexes->first_operand_index);
                *error_found = TRUE;
            }
        }
    }
    else{
        report_error(sentence->line, NOT_AN_OPERAND , counters, indexes->first_operand_index);
        *error_found = TRUE;
    }
}
static void check_number_size(char* line, int index, line_counters* counters, char* error_found){
    if (*(line + index) == '-' || *(line + index) == '+'){
        index++;
    }
    long number = strtod(line + index, NULL);
    if (number > NUMBER_MAX_VAL){
        report_error(line, NUMBER_TOO_LARGE, counters, index);
        *error_found = TRUE;
    }
}
static void check_dest_operand_syntax(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found){
    if (is_operand_proper(sentence, indexes, DEST_OPERAND) == TRUE){
        if (*(sentence->line + indexes->second_operand_index) == '#'){
            check_number_size(sentence->line, indexes->second_operand_index, counters, error_found);
        }
        /*opcodes 1,3: can have any operand except jump to label, which is taken care of next*/
        /*opcodes 0,2-5,12: can't be a number*/
        if (sentence->code_parts.opcode == 0 ||
        (sentence->code_parts.opcode >= 2 && sentence->code_parts.opcode <= 5) ||
        sentence->code_parts.opcode == 12){
            /*checks if number*/
            if (*(sentence->line + indexes->second_operand_index) =='#'){
                report_error(sentence->line, NO_DEST_NUMBER_ALLOWED, counters, indexes->second_operand_index);
                *error_found = TRUE;
            }
        }
        /*opcode 9: can't be a number or a register*/
        if (sentence->code_parts.opcode == 9){
            /*checks if number*/
            if (*(sentence->line + indexes->second_operand_index) == '#'){
                report_error(sentence->line, NO_DEST_NUMBER_ALLOWED, counters, indexes->second_operand_index);
                *error_found = TRUE;
            }
            /*checks if register*/
            if (indexes->second_operand_index == indexes->first_register_index ||
                    indexes->second_operand_index == indexes->second_register_index){
                report_error(sentence->line, NO_DEST_REGISTER_ALLOWED, counters, indexes->second_operand_index);
                *error_found = TRUE;
            }
        }
        else{/*jump to label operand is allowed only for opcode 9*/
            if (*(sentence->line + indexes->second_operand_index) == '&'){
                report_error(sentence->line, NO_DEST_JUMP_LABEL_ALLOWED, counters, indexes->second_operand_index);
                *error_found = TRUE;
            }
        }
    } else{
        report_error(sentence->line, NOT_AN_OPERAND , counters, indexes->second_operand_index);
        *error_found = TRUE;
    }
}
static short int is_operand_proper(line* sentence, line_indexes* indexes, short int source_or_dest){
    short int is_propper = TRUE;
    char* i;
    i = check_operand_first_char_syntax(sentence, indexes, &is_propper, source_or_dest);
    check_operand_body_syntax(i, &is_propper);
    return is_propper;
}
static char* check_operand_first_char_syntax(line* sentence, line_indexes* indexes, short int* is_propper, short int source_or_dest){
    if (source_or_dest == SOURCE_OPERAND){
        if (ILLEGAL_SRC_OPERAND_FIRST_CHAR_CONDITION){
            *is_propper = FALSE;
        }
        return sentence->line + indexes->first_operand_index+1;
    }
    if (source_or_dest == DEST_OPERAND) {
        if (ILLEGAL_DEST_OPERAND_FIRST_CHAR_CONDITION){
            *is_propper = FALSE;
        }
        return sentence->line + indexes->second_operand_index+1;
    }
    return "";
}
static void check_operand_body_syntax(char* i, short int* is_propper){
    while (*i && *i != ' ' && *i != '\t'){
        if (ILLEGAL_OPERAND_BODY_CONDITION){
            *is_propper = FALSE;
        }
        /*if the - or + is not after a #*/
        if ((*i == '-' || *i == '+') && *(i-1) != '#'){
            *is_propper = FALSE;
        }
        i++;
    }
}
static void check_pre_operator_chars(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found){
    int i;
    if (*(sentence->label.name) != '\0' && indexes->colon_index < indexes->operator_index){/*there's a label definition*/
        i = indexes->colon_index+1;
    } else{
        i = indexes->first_char_index;
    }
    while (i < indexes->operator_index){
        /*if the current char is not a white char*/
        if (*(sentence->line + i) != ' ' && *(sentence->line + i) != '\t'){
            report_error(sentence->line, ILLEGAL_CODE_BEFORE_OPERATOR, counters, i);
            *error_found = TRUE;
        }
        i++;
    }
}
/*~~label inspection section~~*/
static void inspect_label(line* sentence, line_indexes* indexes, line_counters* counters, int start_index, char* error_found){
    check_label_length(sentence, indexes, counters, start_index, error_found);
    check_label_syntax(sentence, indexes, counters, start_index, error_found);
}
static void check_label_length(line* sentence, line_indexes* indexes, line_counters* counters, int start_index, char* error_found){
    int label_length;
    int label_index;
    if (*(sentence->label.name) != '\0' && start_index < indexes->colon_index) {/*if that's a label definition*/
        label_length = indexes->colon_index - indexes->first_char_index;
        label_index = indexes->first_char_index;
    } else{
        char* i = sentence->line + indexes->data_index;
        /*count loop*/
        while (*i != ' ' && *i != '\t' && *i){
            i++;
        }
        /*calculates the length with the address of the last char for efficiency*/
        label_length = i - sentence->line - indexes->data_index;
        label_index = indexes->data_index;
    }
    if (label_length > LABEL_MAX_LENGTH-1){
        report_error(sentence->line, LABEL_TOO_LONG, counters, label_index);
        *error_found = TRUE;
    }
}
static void check_label_syntax(line* sentence, line_indexes* indexes, line_counters* counters, int start_index, char* error_found){
    char curr_char = *(sentence->line + start_index);
    char* label_copy = NULL;
    int i = start_index;
    check_label_first_char(sentence, counters, curr_char, i, error_found);
    label_copy = check_label_body(sentence, counters, curr_char, &i, error_found);
    if (*(sentence->label.name) != '\0' && start_index < indexes->colon_index) {/*if it's a label definition*/
        check_label_def_white_chars(sentence, counters, curr_char, i, error_found);
        check_if_label_reserved_word(sentence->label.name, counters, error_found);
    } else{/*if it's not a label definition*/
        check_if_label_reserved_word(label_copy, counters, error_found);
    }
    if (label_copy != NULL){
        free(label_copy);
    }
}
static char* check_label_body(line* sentence, line_counters* counters, char curr_char, int* i, char* error_found){
    int label_length = 0;
    char* label_copy = allocate_memory(LABEL_MAX_LENGTH, CHAR);
    *label_copy = '\0';
    while (curr_char != ' ' && curr_char != '\t' && curr_char && curr_char != ':'){
        if (ILLEGAL_LABEL_CHAR_CONDITION){
            report_error(sentence->line, ILLEGAL_LABEL_SYNTAX, counters, *i);
            *error_found = TRUE;
        }
        if (!(*(sentence->label.name)) && *i < LABEL_MAX_LENGTH){
            *(label_copy + label_length) = curr_char;
        }
        (*i)++;
        curr_char = *(sentence->line + *i);
        label_length++;
    }
    if (!*(sentence->label.name)) {
        *(label_copy + label_length) = '\0';
    }
    return label_copy;
}
static void check_label_first_char(line* sentence, line_counters* counters, char curr_char, int index, char* error_found){
    if (curr_char >= '0' && curr_char <= '9'){
        report_error(sentence->line, LABEL_STARTS_WITH_NUMBER, counters, index);
        *error_found = TRUE;
    }else if (ILLEGAL_LABEL_FIRST_CHAR_CONDITION){
        report_error(sentence->line, LABEL_DOESNT_START_WITH_LETTER, counters, index);
        *error_found = TRUE;
    }
}
static void check_if_label_reserved_word(char* label, line_counters* counters, char* error_found){
    char* reserved_words[NUMBER_OF_RESERVED_WORDS][1] = {{"mov"},{"cmp"},{"add"},{"sub"},{"lea"},{"clr"},{"not"},
                                                         {"inc"},{"dec"},{"jmp"},{"bne"},{"jsr"},{"red"},{"prn"},{"rts"},
                                                         {".data"},{".string"},{".extern"},{".entry"},{"r0"},{"r1"},{"r2"},
                                                         {"r3"},{"r4"},{"r5"},{"r6"},{"r7"}};
    int i = 0;
    while (i < NUMBER_OF_RESERVED_WORDS){
        if (!strcmp(label, reserved_words[i][0])){
            report_error(label, LABEL_IS_RESERVED, counters);
            *error_found = TRUE;
        }
        i++;
    }
}
static void check_label_def_white_chars(line* sentence, line_counters* counters, char curr_char, int index, char* error_found){
    if (curr_char == ' ' || curr_char == '\t') {/*if there is a space in a label definition*/
        report_error(sentence->line, SPACE_IN_LABEL, counters, index);
        *error_found = TRUE;
    }
}
void print_errors_summary(char* file_name, int errors_count){
    printf("\nFILE: %s\n", file_name);
    printf("%d ERRORS WAS FOUND\n", errors_count);
    puts("NO OUTPUT FILES WERE GENERATED"
         "\n*************************************************************************************");
}
static void check_number_appearance(short int *did_number_appeared, char curr_char) {
    if (*did_number_appeared == 0) {
        if (curr_char >= '0' && curr_char <= '9') {
            *did_number_appeared = 1;
        }
    }
}