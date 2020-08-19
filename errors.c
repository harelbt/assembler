#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "errors.h"
#include "in out tools.h"
/*~~general functions~~*/
/*
 * inspect a line for all kinds of errors.
 */
char errors_inspection(line* sentence, line_indexes* indexes, line_counters* counters){
    char error_found = FALSE;/*flag to return*/
    if (*(sentence->label.name) != '\0' && sentence->label.external == FALSE){/*if its a label declaration with a colon ":"*/
        inspect_label(sentence, indexes, counters, indexes->first_char_index, &error_found);
    }
    /*if a label was declared with ":" (not within a string) and no label name was supplied*/
    if (*(sentence->label.name) == '\0' && indexes->colon_index != NOT_FOUND &&
            ((indexes->first_quotation_mark_index != NOT_FOUND && indexes->colon_index < indexes->first_quotation_mark_index) ||
            indexes->first_quotation_mark_index == NOT_FOUND)){
        report_error(sentence->line, MISSING_LABEL, counters, indexes->colon_index);
        error_found = TRUE;
    }
    /*if the line is instruction an order at once*/
    if (sentence->flags.is_data == TRUE && sentence->flags.is_code == TRUE){
        report_error(sentence->line, MIXED_SENTENCE, counters);
        error_found = TRUE;
    } else if (sentence->flags.is_data == TRUE){/*if the line is an order line*/
        inspect_order_line(sentence, indexes, counters, &error_found);
    } else if(sentence->flags.is_code == TRUE){/*if the line is an instruction line*/
        inspect_code_line(sentence, indexes, counters, &error_found);
    } if (sentence->flags.is_data == FALSE && sentence->flags.is_code == FALSE){/*if the line is not an instruction and not an order at once*/
        report_error(sentence->line, NO_SENTENCE_TYPE, counters);
        error_found = TRUE;
    }
    return error_found;
}
/*
 * prints an error to stdout with error number, line number, a line string and in most cases, a char which where the error occurs.
 * extra optional argument is for the index of the error case. this argument is required in some cases and useless in others/
 * requires  error code. 51 error codes are defined as macros in errors.h
 */
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
        case NO_COMMAS_ALLOWED:{
        printf("Commas are not allowed here. line %d.\n", counters->line_number);
        break;
        }
        case EXTRA_COMMAS:{
            printf("Too many commas, only one comma is allowed. line %d.\n", counters->line_number);
            break;
        }
        case COMMA_NOT_IN_PLACE:{
            printf("Comma is not in place, commas need to separate operands. line %d.\n", counters->line_number);
            break;
        }
        case COMMA_REQUIRED:{
            printf("A comma is required to separate operands. line %d.\n", counters->line_number);
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
        case SIGN_NOT_BY_NUM:{
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
            printf("The LABEL is TOO LONG. _label should be in length of 1-31. line %d.\n", counters->line_number);
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
        case LABEL_DOESNT_EXIST:{
            printf("Usage of a label that doesn't exist.\n");
            break;
        }
        default:
            return;
    }
    PRINT_ERROR_DESCRIPTION
}
/*~~order inspection section~~*/
/*
 * inspects order line, from the order itself to the data
 */
static void inspect_order_line(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found){
    if (!is_order_proper(sentence)) {/*if order is not proper*/
        report_error(sentence->line, NO_SUCH_ORDER, counters, indexes->dot_index);
        *error_found = TRUE;
        return;
    }
    detect_string_errors(sentence, indexes, counters, error_found);
    detect_data_errors(sentence, indexes, counters, error_found);
    detect_extern_entry_errors(sentence, indexes, counters, error_found);
}
/*
 * checks if the order in the line (.data/.string/.extern/.entry) is proper, returns TRUE if proper, else FALSE
 */
static char is_order_proper(line* sentence) {
    char is_proper = TRUE;/*to return*/
    if (!strcmp(sentence->data_parts.order, "data") || !strcmp(sentence->data_parts.order, "string") ||
        !strcmp(sentence->data_parts.order, "entry") || !strcmp(sentence->data_parts.order, "extern")) {
        return is_proper;
    }
    is_proper = FALSE;
    return is_proper;
}
/*
 * detects and reports string errors.
 */
static void detect_string_errors(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found) {
    char is_string_order;/*flag to clarify if the order is .string*/
    /*prepares the is_string_order flag*/
    if (!strcmp(sentence->data_parts.order, "string")){
        is_string_order = TRUE;
    } else{
        is_string_order = FALSE;
    }
    /*if a .string order exists without a string afterwards*/
    if (counters->number_of_quotation_marks == 0){
        if (is_string_order == TRUE){
            report_error(sentence->line, ORDER_NO_STRING, counters);
            *error_found = TRUE;
        }
    }
    /*if only opening quote mark exists*/
    if (counters->number_of_quotation_marks == 1) {
        report_error(sentence->line, SINGLE_QUOTMARK, counters, indexes->first_quotation_mark_index);
        *error_found = TRUE;
    }
    /*if a proper string was found, detects errors of a line with a proper string*/
    if (counters->number_of_quotation_marks >= 2) {
        detect_two_quotes_errors(sentence, counters, indexes, is_string_order, error_found);
    }
    /*if .string exists checks the code before the .string order*/
    if (is_string_order == TRUE) {
        check_pre_order_chars(sentence, indexes, counters, error_found);
    }
}
/*
 * detects errors in a line with with a proper string
 */
static void detect_two_quotes_errors(line* sentence, line_counters* counters, line_indexes* indexes, short int is_string_order, char* error_found){
    /*index of the next word after the string in the line*/
    int index_of_word_after_string = find_next_word(sentence->line, indexes->second_quotation_mark_index + 1);
    /*index of the next word after the order in the line*/
    int index_of_word_after_order = find_next_word(sentence->line, indexes->dot_index + (int)strlen(sentence->data_parts.order) + 1);
    if (is_string_order == FALSE) {/*if a string exists without .string order*/
        report_error(sentence->line, STRING_NO_ORDER, counters);
        *error_found = TRUE;
    }
    /*if code wasn't found after the string, the next char is in the '\0' index (the string's length).
     * so checks if the next word index is before the line's length. and if there's a word after, reports an error*/
    if (indexes->second_quotation_mark_index < index_of_word_after_string && index_of_word_after_string < sentence->length){
        report_error(sentence->line, CODE_AFTER_QUOTE, counters, index_of_word_after_string);
        *error_found = TRUE;
    }
    if (is_string_order == TRUE){/*checks string errors with .string order*/
        /*.string exists. so the dot index, which refers to the first dot the refers to the .string index, needs to be before the string*/
        if (indexes->first_quotation_mark_index < indexes->dot_index){
            report_error(sentence->line, STRING_BEFORE_STRING_ORDER, counters, indexes->second_quotation_mark_index);
            *error_found = TRUE;
        }
        /*if there are chars between .string and the string*/
        if (index_of_word_after_order < indexes->first_quotation_mark_index){
            report_error(sentence->line, CHARS_BETWEEN_ORDER_STRING, counters, index_of_word_after_order);
            *error_found = TRUE;
        }
        /*if there is no space before the string*/
        if (indexes->first_quotation_mark_index > 0 && *(sentence->line + indexes->first_quotation_mark_index-1) != ' ' &&
                indexes->first_quotation_mark_index > 0 && *(sentence->line + indexes->first_quotation_mark_index-1) != '\t'){
            report_error(sentence->line, NO_SPACE_BEFORE_QUOTE, counters, indexes->first_quotation_mark_index);
            *error_found = TRUE;
        }
    }
}
/*
 * detects .data order line errors
 */
static void detect_data_errors(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found){
    short int is_data_order;/*flag to clarify if the order is .data*/
    /*prepares the is_data_order flag*/
    if(!strcmp(sentence->data_parts.order,"data")){
        is_data_order = TRUE;
    } else{
        is_data_order = FALSE;
    }
    /*if there's no string (strings errors are being detected in a different function)*/
    if (counters->number_of_quotation_marks == 0){
        if (is_data_order == TRUE){/*if .data exists*/
            /*if data values not proper, errors were found = TRUE, the error report is inside the is_data_values_proper function*/
            if (!is_data_values_proper(sentence, indexes, counters)){
                *error_found = TRUE;
            }
            /*checks for unexpected chars before .data*/
            check_pre_order_chars(sentence, indexes, counters, error_found);
        }
        if (is_data_order == FALSE){
            /*if .data doesn't exist but there are data values, reports error*/
            if (is_data_values_proper(sentence, indexes, counters) == TRUE){
                report_error(sentence->line, DATA_NO_ORDER, counters);
                *error_found = TRUE;
            }
        }
    }
}
/*
 *checks if data values are proper. for example (2,4,6)
 */
static char is_data_values_proper(line* sentence, line_indexes* indexes, line_counters* counters){
    char is_data_values_proper = TRUE;/*to return*/
    char is_data_order;/*flag to clarify if the order is .data*/
    /*sets the is_data_order flag*/
    if(!strcmp(sentence->data_parts.order,"data")){
        is_data_order = TRUE;
    } else{
        is_data_order = FALSE;
    }
    /*if no data exists and .data exists, reports error and is_data_values_proper = FALSE*/
    if (indexes->data_index == NOT_FOUND){/*no data was found*/
        if (is_data_order == TRUE) {
            report_error(sentence->line, ORDER_NO_DATA, counters);
        }
        is_data_values_proper = FALSE;
    }else {/*if the values exists inspect them for errors*/
        if (inspect_data_values(sentence, indexes->data_index, counters) == TRUE) {
            is_data_values_proper = FALSE;
        }
    }
    return is_data_values_proper;
}
/*
 * inspects the data values for example (2,3,8), if errors found returns TRUE else FALSE
 */
static char inspect_data_values(line* sentence, int index, line_counters* counters) {
    char error_found = FALSE;/*to return*/
    char last_char = '\0';
    char curr_char = *(sentence->line + index);
    char is_after_comma = FALSE;/*flag to clarify if a comma is the last non-white char*/
    char did_number_appeared = FALSE;/*flag to clarify if a number already appeared*/
    char is_data_order;/*flag to clarify if .data exists*/
    /*sets is_data_order flag*/
    if (!strcmp(sentence->data_parts.order, "data")) {
        is_data_order = TRUE;
    } else {
        is_data_order = FALSE;
    }
    /*checks valus*/
    while (index < sentence->length) {
        /*checks if current char is illegal*/
        if (ILLEGAL_DATA_CHAR_CONDITION) {
            /*reports an error only if .data exists. if .data doesn't exist, reports a different error in is_data_values_proper function*/
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
/*
 * data values check for inspect_data_values function
 */
static void values_check(line* sentence, line_counters* counters, char curr_char, int last_char, short int is_after_comma,
                         short int is_data_order, char* error_found, short int did_number_appeared, int index) {
        if (is_after_comma == TRUE && curr_char == ',') {/*if two comma exists without a number between*/
            if (is_data_order == TRUE){/*reports an error if .data exists*/
                report_error(sentence->line, NO_NUMBERS_BETWEEN_COMMAS, counters, index);
            }
            *error_found = TRUE;
        }
        if (curr_char == ',' && did_number_appeared == FALSE) {/*if a comma appeared before a number*/
            if (is_data_order == TRUE) {/*reports an error if .data exists*/
                report_error(sentence->line, COMMA_NO_NUMBERS, counters, index-1);
            }
            *error_found = TRUE;
        }
        if ((curr_char == '-' || curr_char == '+') && ((index + 1 != sentence->length && *(sentence->line + index + 1) < '0'
        || *(sentence->line + index + 1) > '9') || index + 1 == sentence->length)){/*if a '-' or '+' was written not by a number*/
            report_error(sentence->line, SIGN_NOT_BY_NUM, counters, index);
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
            if (index == sentence->length-1 && (curr_char == ',' || is_after_comma == TRUE)){/*if no number shows up after a comma*/
                report_error(sentence->line, COMMA_NO_FOLLOWING_NUMBER, counters, index);
                *error_found = TRUE;
            }
        }
        if ((curr_char == '-' || curr_char == '+') && *(sentence->line + index - 1) != ' ' &&
                *(sentence->line + index - 1) != '\t' && *(sentence->line + index - 1) != ','){/*if '-' or '+' is not in a proper place*/
            report_error(sentence->line, ARITHMETIC_SIGN_NOT_IN_PLACE, counters, index);
            *error_found = TRUE;
        }
    }
/*
* turns on and off "is_after_comma" flag for inspect_data_values function
*/
static void check_if_after_comma(const char *curr_char, char *is_after_comma) {
    if (*curr_char == ',') {
        *is_after_comma = TRUE;
    } else if (*curr_char != ' ' && *curr_char != '\t') {
        *is_after_comma = FALSE;
    }
}
/*
 * updates loop variables for inspect_data_values function
 */
static void update_loop_data_inspection_variables(line* sentence, char* curr_char, char* last_char, int* index){
    if (*index < sentence->length) {
        (*index)++;
        *last_char = *curr_char;
        *curr_char = *(sentence->line + (*index));
    }
}
/*
 * checks errors for .entry and .extern lines
 */
static void detect_extern_entry_errors(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found) {
    /*checks errors only if the order in the line is .extern or .entry*/
    if (!strcmp(sentence->data_parts.order, "entry") || !strcmp(sentence->data_parts.order, "extern")) {
        inspect_data_label(sentence, indexes, counters, error_found);/*inspects the label after the order for errors*/
        check_pre_order_chars(sentence, indexes, counters, error_found);/*checks the code before the order for errors*/
        check_after_data_label_chars(sentence, counters, indexes, error_found);/*checks the code after the label for errors*/
    }
}
/*
 * inspect a label that comes as data after .extern or .entry
 */
static void inspect_data_label(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found){
    int order_end_index = indexes->dot_index + (int) strlen(sentence->data_parts.order);/*end index of .extern or .entry*/
    if (indexes->data_index == NOT_FOUND) {/*label wasn't found. reports an error*/
        report_error(sentence->line, EXTERN_ENTRY_NO_LABEL, counters, order_end_index);
        *error_found = TRUE;
    } else {/*if a label was found, inspects the label for errors*/
        inspect_label(sentence, indexes, counters, indexes->data_index, error_found);
    }
}
/*
 * checks and reports if there are characters after a label in a .extern ot .entry line
 */
static void check_after_data_label_chars(line* sentence, line_counters* counters, line_indexes* indexes, char* error_found){
    char data_ended = FALSE;/*flag to clarify if the data ended*/
    char* i = sentence->line + indexes->data_index;/*the string that is being checked starts where the given label starts*/
    while (*i) {/*while line hasn't ended*/
        if (!data_ended && (*i == ' ' || *i == '\t')){
            data_ended = TRUE;
        }
        if (data_ended == TRUE && *i != ' ' && *i != '\t') {/*if a character appeared after the label reports an error*/
            report_error(sentence->line, UNEXPECTED_CHARACTER, counters, *i, i - sentence->line);
            *error_found = TRUE;
        }
        i++;
    }
}
/*
 * checks for illegal characters before an order in an order line
 */
static void check_pre_order_chars(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found){
    char error_code;/*the error code to report if an error was found*/
    /*sets "error code" according to the order*/
    if (!strcmp(sentence->data_parts.order, "data")){
        error_code = CHARS_BEFORE_DATA_ORDER;
    }
    if (!strcmp(sentence->data_parts.order, "string")){
        error_code = CHARS_BEFORE_STRING_ORDER;
    }
    if (!strcmp(sentence->data_parts.order, "extern") || !strcmp(sentence->data_parts.order, "entry")){
        error_code = CHARS_BEFORE_EXTERN_OR_ENTRY;
    }
    /*we are here only if the first dot starts proper order, so the dot index surely is the index of the order*/
    /*checks if there are chars between the label and the order*/
    if (*(sentence->label.name) != '\0' && indexes->colon_index < indexes->dot_index){/*if there's a label declaration*/
        /*the index of the first non-white character after the label declaration*/
        int index_of_word_after_label = find_next_word(sentence->line, indexes->colon_index+1);
        if (*(sentence->line + index_of_word_after_label) != '.') {/*if after the label declaration it's not the order, reports an error*/
            report_error(sentence->line, error_code, counters, index_of_word_after_label);
            *error_found = TRUE;
        }
    } else if (*(sentence->line + indexes->first_char_index) != '.'){/*if there's no label declaration but the order is not the first in the line*/
        report_error(sentence->line, error_code, counters, indexes->first_char_index);
        *error_found = TRUE;
    }
}
/*~~code inspection section~~*/
/*
 * checks a code line for errors
 */
static void inspect_code_line(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found) {
    /*the functions below report errors and return TRUE if an error found*/
    /*inspects the number of operands*/
    char is_operand_count_not_okay = check_operands_count(sentence, indexes, counters, error_found);
    /*inspects the number of operators*/
    char is_operators_count_not_okay = check_operators_count(sentence, indexes, counters, error_found);
    /*if the operands and operators count okay checks the operands syntax and pre operators chars errors */
    if (!is_operand_count_not_okay && !is_operators_count_not_okay) {
        check_operands_syntax(sentence, indexes, counters, error_found);
        check_pre_operator_chars(sentence, indexes, counters, error_found);
    }
}
/*
 * checks if the number of operands is legal for the operator, reports errors. returns TRUE if error found, else returns FALSE
 */
static char check_operands_count(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found){
    char is_error = FALSE;/* to return*/
    switch (sentence->code_parts.opcode) {
        case 0:{}
        case 1:{}
        case 2:{}
        case 3:{}
        case 4:{/*this case and all the cases above requires exactly 2 operands*/
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
        case 13:{/*all the cases from case 5 to case 13 requires exactly 1 operand*/
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
        case 15:{/*cases 14 and 15exactly 0 operands*/
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
/*
 * checks if the number of operators is greater than 1, if so reports an error and returns TRUE. else returns FALSE
 */
static char check_operators_count(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found){
    char is_error = FALSE;/*to return*/
    if (counters->number_of_operators > 1){
        report_error(sentence->line, EXTRA_OPERATORS, counters);
        *error_found = TRUE;
        is_error = TRUE;
    }
    return is_error;
}
/*
 * checks operands syntax according to the operator and commas conditions, reports errors
 */
static void check_operands_syntax(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found){
    switch (sentence->code_parts.opcode) {
        case 0:{}
        case 1:{}
        case 2:{}
        case 3:{}
        case 4:{
            check_source_operand_syntax(sentence, indexes, counters, error_found);
            check_dest_operand_syntax(sentence, indexes, counters, error_found);
            /*checks if there's exactly one comma*/
            if (counters->number_of_commas > 1){
                report_error(sentence->line, EXTRA_COMMAS, counters);
                *error_found = TRUE;
            }
            if (counters->number_of_commas == 0){
                report_error(sentence->line, COMMA_REQUIRED, counters);
                *error_found = TRUE;
            }/*checks the last comma position(before first operand or after last operand is an error)*/
            if (indexes->last_comma_index != NOT_FOUND &&
            (indexes->last_comma_index < indexes->first_operand_index || indexes->last_comma_index > indexes->second_operand_index)){
                report_error(sentence->line, COMMA_NOT_IN_PLACE, counters);
                *error_found = TRUE;
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
            check_dest_operand_syntax(sentence, indexes, counters, error_found);
        }
        default:{/*for operators with only 1 operand, comma is not allowed*/
            if (counters->number_of_commas > 0){
                report_error(sentence->line, NO_COMMAS_ALLOWED, counters);
                *error_found = TRUE;
            }
            return;
        }
    }
}
/*
 *checks source operand syntax and reports errors
 */
static void check_source_operand_syntax(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found){
    /*if the operand is proper. if so, checks if it fits the operator*/
    if (is_operand_proper(sentence, indexes, SOURCE_OPERAND) == TRUE) {
        /*if the operand is a number, checks the number size*/
        if (*(sentence->line + indexes->first_operand_index) == '#'){
            check_number_size(sentence->line, indexes->first_operand_index + 1, counters, error_found);
        }
        /*"jump to label" is not allowed as a source operand*/
        if (*(sentence->line+indexes->first_operand_index) == '&'){
            report_error(sentence->line, NO_SOURCE_JUMP_LABEL_ALLOWED, counters, indexes->first_operand_index);
            *error_found = TRUE;
        }
        /*opcode 4 has more restrictions, no number or register is allowed*/
        if (sentence->code_parts.opcode == 4){
            if (*(sentence->line + indexes->first_operand_index) == '#') {
                report_error(sentence->line, NO_SOURCE_NUMBER_ALLOWED, counters, indexes->first_operand_index);
                *error_found = TRUE;
            }
            if (indexes->first_operand_index == indexes->first_register_index){/*first operand can't be second register*/
                report_error(sentence->line, NO_SOURCE_REGISTER_ALLOWED, counters, indexes->first_operand_index);
                *error_found = TRUE;
            }
        }
    }else{/*if operand is not proper, reports an error*/
        report_error(sentence->line, NOT_AN_OPERAND , counters, indexes->first_operand_index);
        *error_found = TRUE;
    }
}
/*
 * gets a number in char*, number can be with a sign, and reports an error if number is too large
 */
static void check_number_size(char* line, int index, line_counters* counters, char* error_found){
    long long number;
    /*skips the sign*/
    if (*(line + index) == '-' || *(line + index) == '+'){
        index++;
    }
    number = strtod(line + index, NULL);
    if (number > NUMBER_MAX_VAL){/*checks number size*/
        report_error(line, NUMBER_TOO_LARGE, counters, index);
        *error_found = TRUE;
    }
}
/*
 * checks destination operand syntax and reports errors
 */
static void check_dest_operand_syntax(line* sentence, line_indexes* indexes, line_counters* counters, char* error_found){
    /*if the operand is proper. if so, checks if it fits the operator*/
    if (is_operand_proper(sentence, indexes, DEST_OPERAND) == TRUE){
        if (*(sentence->line + indexes->second_operand_index) == '#'){
            /*if the operand is a number, checks the number size*/
            check_number_size(sentence->line, indexes->second_operand_index + 1, counters, error_found);
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
        }else{/*jump to label operand is allowed only for opcode 9*/
            if (*(sentence->line + indexes->second_operand_index) == '&'){
                report_error(sentence->line, NO_DEST_JUMP_LABEL_ALLOWED, counters, indexes->second_operand_index);
                *error_found = TRUE;
            }
        }
    } else{/*if operand isn't proper*/
        report_error(sentence->line, NOT_AN_OPERAND , counters, indexes->second_operand_index);
        *error_found = TRUE;
    }
}
/*
 * returns TRUE if operand is proper, else returns FALSE
 */
static char is_operand_proper(line* sentence, line_indexes* indexes, char source_or_dest){
    char is_propper = TRUE;/*to return*/
    char* i;/*gets the line from the second char of the operand, used to continue to check the operand's body*/
    i = check_operand_first_char_syntax(sentence, indexes, &is_propper, source_or_dest);
    check_operand_body_syntax(i, &is_propper);
    return is_propper;
}
/*
 * checks the syntax of the first char of the operand, sets the "is_propper" flag that is given as an argument.
 * requires source_or_dest flag with macros: SOURCE_OPERAND, DEST_OPERAND. due to different checks.
 * returns the line from the second char of the operand
 */
static char* check_operand_first_char_syntax(line* sentence, line_indexes* indexes, char* is_propper, char source_or_dest){
    /*checks the first according to the operand (source or destination)*/
    if (source_or_dest == SOURCE_OPERAND){
        if (ILLEGAL_SRC_OPERAND_FIRST_CHAR_CONDITION){/*check source operand's first char*/
            *is_propper = FALSE;
        }
        return sentence->line + indexes->first_operand_index+1;/*returns char* from one char after the first operand's char*/
    }
    if (source_or_dest == DEST_OPERAND) {
        if (ILLEGAL_DEST_OPERAND_FIRST_CHAR_CONDITION){/*check destination operand's first char*/
            *is_propper = FALSE;
        }
        return sentence->line + indexes->second_operand_index+1;/*returns char* from one char after the first operand's char*/
    }
    return "";/*in case "source_or_dest" argument wasn't supplied correctly*/
}
/*
 * checks the syntax of the operand's body. sets the "is_propper" flag that is given as an argument.
 */
static void check_operand_body_syntax(char* i, char* is_proper){
    while (*i && *i != ' ' && *i != '\t' && *i != ','){/*runs as long as the operand isn't ended*/
        if (ILLEGAL_OPERAND_BODY_CONDITION){
            *is_proper = FALSE;
        }
        /*if the - or + is not after a # the operand isn't proper*/
        if ((*i == '-' || *i == '+') && *(i-1) != '#'){
            *is_proper = FALSE;
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
        /*calculates the length with the label_address of the last char for efficiency*/
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
static void check_number_appearance(char* did_number_appeared, char curr_char) {
    if (*did_number_appeared == 0) {
        if (curr_char >= '0' && curr_char <= '9') {
            *did_number_appeared = 1;
        }
    }
}