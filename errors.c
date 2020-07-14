#include "errors.h"
#include "helpfunctions.h"
short int errors_inspection(line sentence, line_marks_index indexes, line_marks_counter counters){
    short int error_found = 0;
    if (sentence.flags.is_data == TRUE && sentence.flags.is_code == TRUE){
        report_error(sentence.line, MIXED_SENTENCE, counters);
        error_found = 1;
    } else if (sentence.flags.is_data == TRUE){
        error_found = inspect_order_line(sentence, indexes, counters);
    } else if(sentence.flags.is_code == TRUE){
        error_found = inspect_code_line(sentence, indexes, counters);
    } if (sentence.flags.is_data == FALSE && sentence.flags.is_code == FALSE){
        report_error(sentence.line, NO_SENTENCE_TYPE, counters);
        error_found = 1;
    }
    return error_found;
}
static void report_error(char* line, short int error_code, line_marks_counter counters, ...){
    printf("ERROR: ");
    switch (error_code) {
        case UNEXPECTED_CHARACTER:{
            int unexpected;
            va_list argp;
            va_start(argp, counters);
            unexpected = va_arg(argp, int);
            printf("%c is an unexpected character in line %d.\n", unexpected, counters.line_number);
            va_end(argp);
            break;
        }
        case UNEXPECTED_SEMICOLON:{
            printf("unexpected semicolon in line %d.\n", counters.line_number);
            break;
        }
        case UNEXPECTED_HASHMARK:{
            printf("unexpected hash mark in line %d.\n", counters.line_number);
            break;
        }
        case UNEXPECTED_REGISTER:{
            printf("unexpected register in line %d.\n", counters.line_number);
            break;
        }
        case UNEXPECTED_QUOT_MARK:{
            printf("unexpected quotation mark in line %d.\n", counters.line_number);
            break;
        }
        case REGISTER_NO_OPERATOR:{
            printf("register is written but no operator found in line %d.\n", counters.line_number);
            break;
        }
        case HASHMARK_NO_OPERATOR:{
            printf("hash mark is written but no operator found in line %d.\n", counters.line_number);
            break;
        }
        case EXTRA_OPERATORS:{
            printf("Extra operator in line %d.\n", counters.line_number);
            break;
        }
        case MIXED_SENTENCE:{
            printf("Mixed code and data order in line %d.\n", counters.line_number);
            break;
        }
        case NO_SENTENCE_TYPE:{
            printf("Sentence is not code or data order in line %d. check the syntax.\n", counters.line_number);
            break;
        }
        case SINGLE_QUOTMARK:{
            printf("opening quote mark without closing quote mark in line %d.\n", counters.line_number);
            break;
        }
        case STRING_NO_ORDER:{
            printf("string without a .string order before in line %d.\n", counters.line_number);
            break;
        }
        case CODE_AFTER_QUOTE:{
            printf("code was found after a string in line %d.\n", counters.line_number);
            break;
        }
        case ORDER_NO_STRING:{
            printf(".string order without a string afterwards in line %d.\n", counters.line_number);
            break;
        }
        case CHARS_BETWEEN_ORDER_STRING:{
            printf("Unexpected code between the .string order and the string afterwards in line %d.\n", counters.line_number);
            break;
        }
        case CHARS_BETWEEN_LABEL_STRINGORDER:{
            printf("Unexpected code between the label and the .string order in line %d.\n", counters.line_number);
            break;
        }
        case CHARS_BEFORE_STRING_ORDER:{
            printf("Unexpected code before the .string order in line %d.\n", counters.line_number);
            break;
        }
        case DATA_NO_ORDER:{
            printf("Data supplied without .data order in line %d.\n", counters.line_number);
            break;
        }
        case ORDER_NO_DATA:{
            printf(".data order supplied without data in line %d.\n", counters.line_number);
            break;
        }
        case NO_COMMA_BETWEEN:{
            printf("No comma is placed between numbers in line %d.\n", counters.line_number);
            break;
        }
        case NO_NUMBERS_BETWEEN_COMMAS:{
            printf("No numbers between two commas in line %d.\n", counters.line_number);
            break;
        }
        default:
            return;
    }
    puts(line);
}
static short int inspect_order_line(line sentence, line_marks_index indexes, line_marks_counter counters){
    short int error_found = 0;
    //if (!proper_order_check){
    //report_error();
    //error_found = 1;
    //return error_found
    //}
    if (detect_string_errors(sentence, indexes, counters) == 1){
        error_found = 1;
    }
    if (detect_data_errors(sentence, indexes, counters) == 1){
        error_found = 1;
    }
    return error_found;
}
static short int inspect_code_line(line sentence, line_marks_index indexes, line_marks_counter counters){

}
static short int detect_string_errors(line sentence, line_marks_index indexes, line_marks_counter counters) {
    short int error_found = 0;
    if (counters.number_of_quotation_marks == 0){
        if (!strcmp(sentence.data_parts.order, "string")){
            report_error(sentence.line, ORDER_NO_STRING, counters);
            error_found = 1;
        }
    }
    /**/
    if (counters.number_of_quotation_marks == 1) {
        report_error(sentence.line, SINGLE_QUOTMARK, counters);
        error_found = 1;
    }
    /**/
    if (counters.number_of_quotation_marks >= 2) {
        if (strcmp(sentence.data_parts.order, "string") != 0) {
            report_error(sentence.line, STRING_NO_ORDER, counters);
            error_found = 1;
        }
        if (indexes.second_quotation_mark_index < find_next_word(sentence.line, indexes.second_quotation_mark_index+1)){
            report_error(sentence.line, CODE_AFTER_QUOTE, counters);
            error_found = 1;
        }
        if (!strcmp(sentence.data_parts.order, "string")){
            if (indexes.first_quotation_mark_index < indexes.dot_index){
                report_error(sentence.line, CHARS_BEFORE_STRING_ORDER, counters);
                error_found = 1;
            }
            if (*(sentence.line + find_next_word(sentence.line, indexes.dot_index+7)) != '\"') {
                report_error(sentence.line, CHARS_BETWEEN_ORDER_STRING, counters);
                error_found = 1;
            }
            if (counters.number_of_colons > 0 &&
            indexes.dot_index > indexes.colon_index &&
            *(sentence.line + find_next_word(sentence.line, indexes.colon_index + 1)) != '.'){
                report_error(sentence.line, CHARS_BETWEEN_LABEL_STRINGORDER, counters);
                error_found = 1;
            }
            if ((indexes.colon_index > indexes.dot_index || indexes.colon_index == -1) &&
            indexes.dot_index != indexes.first_char_index){
                report_error(sentence.line, CHARS_BEFORE_STRING_ORDER, counters);
                error_found = 1;
            }
        }
    }
    return error_found;
}
static short int detect_data_errors(line sentence, line_marks_index indexes, line_marks_counter counters){
    short int error_found = 0;
    if (counters.number_of_quotation_marks == 0){
        if (!(strcmp(sentence.data_parts.order,"data"))){
            if (!is_data_values_proper(sentence, indexes, counters)){
                error_found = 1;
            }
        }
        if ((strcmp(sentence.data_parts.order,"data")) != 0){
            if (is_data_values_proper(sentence, indexes, counters) == 1){
                report_error(sentence.line, DATA_NO_ORDER, counters);
                error_found = 1;
            }
        }
    }
    return error_found;
}
static short int is_data_values_proper(line sentence, line_marks_index indexes, line_marks_counter counters){
    int val_index = find_data_values(sentence.line,indexes.dot_index);
    short int is_data_proper = 1;
    if (val_index == -1){
        if (!strcmp(sentence.data_parts.order,"data")) {
            report_error(sentence.line, ORDER_NO_DATA, counters);
        }
        is_data_proper = 0;
    }
    if (inspect_data_values(sentence.line, val_index, counters) == 1){
        is_data_proper = 0;
    }
    return is_data_proper;
}
static short int inspect_data_values(char* line, int index, line_marks_counter counters){
    short int error_found = 0;
    int str_length = (int)strlen(line);
    char last_char = '\0';
    char curr_char = *(line + index);
    short int is_after_comma = 0;
    short int did_number_appeared = 0;
    while (index < str_length){
        /*checks if current char is illegal*/
        if (curr_char != ' ' && curr_char != '\t' && curr_char != ',' && curr_char != '-' && curr_char != '+'
        && (curr_char < '0' || curr_char > '9')){
            report_error(line, UNEXPECTED_CHARACTER, counters, curr_char);
            error_found = 1;
        } else {/*if current char is legal*/
            if (curr_char == ',') {
                is_after_comma = 1;
            }
            else if (curr_char != ' ' && curr_char != '\t'){
                is_after_comma = 0;
            }
            if (did_number_appeared == 0) {
                if (curr_char > '0' && curr_char < '9') {
                    did_number_appeared = 1;
                }
            }
            if (is_after_comma == 1 && did_number_appeared == 0){
                report_error(line, COMMA_NO_NUMBERS, counters);
                error_found = 1;
            }
            if (did_number_appeared == 1) {
                if ((last_char == ' ' || last_char == '\t') && curr_char != ' ' && curr_char != '\t' &&
                    is_after_comma == 0) {
                    report_error(line, NO_COMMA_BETWEEN, counters);
                    error_found = 1;
                }
            }
            if (is_after_comma == 1 && curr_char == ','){
                report_error(line, NO_NUMBERS_BETWEEN_COMMAS, counters);
                error_found = 1;
            }
        }
        index++;
        last_char = curr_char;
        curr_char = *(line + index);
    }

    return error_found;
}