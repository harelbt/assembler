#include "errors.h"
#include "helpfunctions.h"
/*~~general functions~~*/
short int errors_inspection(line* sentence, line_marks_index indexes, line_marks_counter* counters){
    short int error_found = 0;
    if (*(sentence->label.name) != '\0'){
        inspect_label(sentence, indexes, counters, indexes.first_char_index, &error_found);
    }
    if (*(sentence->label.name) == '\0' && indexes.colon_index != -1 &&
            ((indexes.first_quotation_mark_index != -1 && indexes.colon_index < indexes.first_quotation_mark_index) ||
            indexes.first_quotation_mark_index == -1)){
        report_error(sentence->line, MISSING_LABEL, counters, indexes.colon_index);
        error_found = 1;
    }
    if (sentence->flags.is_data == TRUE && sentence->flags.is_code == TRUE){
        report_error(sentence->line, MIXED_SENTENCE, counters);
        error_found = 1;
    } else if (sentence->flags.is_data == TRUE){
        error_found = inspect_order_line(sentence, indexes, counters);
    } else if(sentence->flags.is_code == TRUE){
        inspect_code_line(*sentence, indexes, counters, &error_found);
    } if (sentence->flags.is_data == FALSE && sentence->flags.is_code == FALSE){
        report_error(sentence->line, NO_SENTENCE_TYPE, counters);
        error_found = 1;
    }
    return error_found;
}
static void report_error(char* line, short int error_code, line_marks_counter* counters, ...){
    int unexpected;
    short int print_char_indication = 0;
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
        case UNEXPECTED_REGISTER:{
            printf("Unexpected register in line %d.\n", counters->line_number);
            break;
        }
        case REGISTER_NO_OPERATOR:{
            printf("Register is written but no operator found in line %d.\n", counters->line_number);
            break;
        }
        case HASHMARK_NO_OPERATOR:{
            printf("Hash mark is written but no operator found in line %d.\n", counters->line_number);
            break;
        }
        case EXTRA_OPERATORS:{
            printf("Extra operator in line %d.\n", counters->line_number);
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
            print_char_indication = 1;
            printf("Opening quote mark without closing quote mark in line %d.\n", counters->line_number);
            break;
        }
        case STRING_NO_ORDER:{
            printf("String without a .string order before in line %d.\n", counters->line_number);
            break;
        }
        case CODE_AFTER_QUOTE:{
            print_char_indication = 1;
            printf("Code was found after a string in line %d.\n", counters->line_number);
            break;
        }
        case ORDER_NO_STRING:{
            printf(".string order without a string afterwards. line %d.\n", counters->line_number);
            break;
        }
        case CHARS_BETWEEN_ORDER_STRING:{
            print_char_indication = 1;
            printf("Unexpected code between the .string order and the string afterwards. line %d.\n", counters->line_number);
            break;
        }
        case NO_SPACE_BEFORE_QUOTE:{
            print_char_indication = 1;
            printf("A string needs to be written after a space or spaces. line %d.\n", counters->line_number);
            break;
        }
        case CHARS_BEFORE_STRING_ORDER:{
            print_char_indication = 1;
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
            print_char_indication = 1;
            printf("Unexpected code before .data order in line %d.\n", counters->line_number);
            break;
        }
        case NO_COMMA_BETWEEN:{
            print_char_indication = 1;
            printf("No comma is placed between numbers in line %d.\n", counters->line_number);
            break;
        }
        case COMMA_NO_NUMBERS:{
            print_char_indication = 1;
            printf("Data can't begin with ',' without a number before. line %d.\n", counters->line_number);
            break;
        }
        case COMMA_NO_FOLLOWING_NUMBER:{
            print_char_indication = 1;
            printf("No number was found after ','. line %d.\n", counters->line_number);
            break;
        }
        case NO_NUMBERS_BETWEEN_COMMAS:{
            print_char_indication = 1;
            printf("No numbers between two commas in line %d.\n", counters->line_number);
            break;
        }
        case NO_SUCH_ORDER:{
            print_char_indication = 1;
            printf("Illegal order in line %d.\n", counters->line_number);
            break;
        }
        case STRING_BEFORE_STRING_ORDER:{
            print_char_indication = 1;
            printf("String before .string order. line %d.\n", counters->line_number);
            break;
        }
        case CHARS_BEFORE_EXTERN_OR_ENTRY:{
            print_char_indication = 1;
            printf("Unexpected code before .extern or .entry order in line %d.\n", counters->line_number);
            break;
        }
        case EXTERN_ENTRY_NO_LABEL:{
            print_char_indication = 1;
            printf(".extern or .entry order but no label was found. line %d.\n", counters->line_number);
            break;
        }
        case LABEL_TOO_LONG:{
            print_char_indication = 1;
            printf("The LABEL is TOO LONG. _label should be in length of 1-31_. line %d.\n", counters->line_number);
            break;
        }
        case MISSING_LABEL:{
            print_char_indication = 1;
            printf("The LABEL is MISSING. _label should be in length of 1-31_. line %d.\n", counters->line_number);
            break;
        }
        case LABEL_STARTS_WITH_NUMBER:{
            print_char_indication = 1;
            printf("A label can begin only with a non number character. line %d.\n", counters->line_number);
            break;
        }
        case LABEL_NOT_BY_COLON:{
            print_char_indication = 1;
            printf("The label is not by the colon. line %d.\n", counters->line_number);
            break;
        }
        case SPACE_IN_LABEL:{
            print_char_indication = 1;
            printf("White chars are not allowed in a label or before a colon. line %d.\n", counters->line_number);
            break;
        }
        case LABEL_DOESNT_START_WITH_LETTER:{
            print_char_indication = 1;
            printf("A label can begin only with a letter. line %d.\n", counters->line_number);
            break;
        }
        case ILLEGAL_LABEL_SYNTAX:{
            print_char_indication = 1;
            printf("Illegal character in label. line %d.\n", counters->line_number);
            break;
        }
        default:
            return;
    }
    PRINT_ERROR_DESCRIPTION
}
/*~~order inspection section~~*/
static short int inspect_order_line(line* sentence, line_marks_index indexes, line_marks_counter* counters){
    short int error_found = 0;
    if (!is_order_proper(*sentence)) {
        report_error(sentence->line, NO_SUCH_ORDER, counters, indexes.dot_index);
        error_found = 1;
        return error_found;
    }
    detect_string_errors(*sentence, indexes, counters, &error_found);
    detect_data_errors(*sentence, indexes, counters, &error_found);
    detect_extern_entry_errors(sentence, indexes, counters, &error_found);
    return error_found;
}
static short int is_order_proper(line sentence) {
    short int is_proper = 1;
    if (!strcmp(sentence.data_parts.order, "data") || !strcmp(sentence.data_parts.order, "string") ||
        !strcmp(sentence.data_parts.order, "entry") || !strcmp(sentence.data_parts.order, "extern") ||
            !strcmp(sentence.data_parts.order, "stop")) {
        return is_proper;
    }
    is_proper = 0;
    return is_proper;

}
static void detect_string_errors(line sentence, line_marks_index indexes, line_marks_counter* counters, short int* error_found) {
    short int is_string_order;
    /**/
    if (!strcmp(sentence.data_parts.order, "string")){
        is_string_order = 1;
    } else{
        is_string_order = 0;
    }
    /**/
    if (counters->number_of_quotation_marks == 0){
        if (is_string_order == 1){
            report_error(sentence.line, ORDER_NO_STRING, counters);
            *error_found = 1;
        }
    }
    /**/
    if (counters->number_of_quotation_marks == 1) {
        report_error(sentence.line, SINGLE_QUOTMARK, counters, indexes.first_quotation_mark_index);
        *error_found = 1;
    }
    /**/
    if (counters->number_of_quotation_marks >= 2) {
        detect_two_quotes_errors(sentence, counters, indexes, is_string_order, error_found);
    }
    if (is_string_order == 1) {
        check_pre_order_chars(sentence, indexes, counters, error_found);
    }
}
static void detect_two_quotes_errors(line sentence, line_marks_counter* counters, line_marks_index indexes, short int is_string_order, short int* error_found){
    int char_after_string_index = find_next_word(sentence.line, indexes.second_quotation_mark_index+1);
    int char_after_order_index = find_next_word(sentence.line, indexes.dot_index+(int)strlen(sentence.data_parts.order)+1);
    if (is_string_order == 0) {
        report_error(sentence.line, STRING_NO_ORDER, counters);
        *error_found = 1;
    }
    if (indexes.second_quotation_mark_index < char_after_string_index && char_after_string_index < sentence.length){
        report_error(sentence.line, CODE_AFTER_QUOTE, counters, char_after_string_index);
        *error_found = 1;
    }
    if (is_string_order == 1){
        if (indexes.first_quotation_mark_index < indexes.dot_index){
            report_error(sentence.line, STRING_BEFORE_STRING_ORDER, counters, indexes.second_quotation_mark_index);
            *error_found = 1;
        }
        if (char_after_order_index < indexes.first_quotation_mark_index){
            report_error(sentence.line, CHARS_BETWEEN_ORDER_STRING, counters, char_after_order_index);
            *error_found = 1;
        }
        if (indexes.first_quotation_mark_index > 0 && *(sentence.line + indexes.first_quotation_mark_index-1) != ' ' &&
                indexes.first_quotation_mark_index > 0 && *(sentence.line + indexes.first_quotation_mark_index-1) != '\t'){
            report_error(sentence.line, NO_SPACE_BEFORE_QUOTE, counters, indexes.first_quotation_mark_index);
        }
    }
}
static void detect_data_errors(line sentence, line_marks_index indexes, line_marks_counter* counters, short int* error_found){
    short int is_data_order;
    if(!strcmp(sentence.data_parts.order,"data")){
        is_data_order = 1;
    } else{
        is_data_order = 0;
    }
    if (counters->number_of_quotation_marks == 0){
        if (is_data_order == 1){
            if (!is_data_values_proper(sentence, indexes, counters)){
                *error_found = 1;
            }
            /*unexpected chars check*/
            check_pre_order_chars(sentence, indexes, counters, error_found);
        }
        if (is_data_order == 0){
            if (is_data_values_proper(sentence, indexes, counters) == 1){
                report_error(sentence.line, DATA_NO_ORDER, counters);
                *error_found = 1;
            }
        }
    }
}
static short int is_data_values_proper(line sentence, line_marks_index indexes, line_marks_counter* counters){
    short int is_data_values_proper = 1;
    short int is_data_order;
    if(!strcmp(sentence.data_parts.order,"data")){
        is_data_order = 1;
    } else{
        is_data_order = 0;
    }
    if (indexes.data_index == -1){/*no data was found*/
        if (is_data_order == 1) {
            report_error(sentence.line, ORDER_NO_DATA, counters);
        }
        is_data_values_proper = 0;
    }else {
        if (inspect_data_values(sentence, indexes.data_index, counters) == 1) {
            is_data_values_proper = 0;
        }
    }
    return is_data_values_proper;
}
static short int inspect_data_values(line sentence, int index, line_marks_counter* counters) {
    short int error_found = 0;
    char last_char = '\0';
    char curr_char = *(sentence.line + index);
    short int is_after_comma = 0;
    short int did_number_appeared = 0;
    short int is_data_order;
    if (!strcmp(sentence.data_parts.order, "data")) {
        is_data_order = 1;
    } else {
        is_data_order = 0;
    }
    while (index < sentence.length) {
        /*checks if current char is illegal*/
        if (curr_char != ' ' && curr_char != '\t' &&
        curr_char != ',' && curr_char != '-' && curr_char != '+'
            && (curr_char < '0' || curr_char > '9')) {
            if (is_data_order == 1) {
                report_error(sentence.line, UNEXPECTED_CHARACTER, counters, curr_char, index);
            }
            error_found = 1;/*indicates that in general, the values are not proper*/
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
static void
values_check(line sentence, line_marks_counter *counters, char curr_char, int last_char, short int is_after_comma,
             short int is_data_order, short int *error_found, short int did_number_appeared, int index) {
        if (is_after_comma == 1 && curr_char == ',') {
            if (is_data_order == 1) {
                report_error(sentence.line, NO_NUMBERS_BETWEEN_COMMAS, counters, index);
            }
            *error_found = 1;
        }
        if (curr_char == ',' == 1 && did_number_appeared == 0) {
            if (is_data_order == 1) {
                report_error(sentence.line, COMMA_NO_NUMBERS, counters, index-1);
            }
            *error_found = 1;
        }
        if (did_number_appeared == 1) {
            if ((last_char == ' ' || last_char == '\t') && curr_char != ' ' && curr_char != '\t' &&
                is_after_comma == 0 && curr_char != ',') {/*if no comma was found before a number*/
                if (is_data_order == 1) {
                    report_error(sentence.line, NO_COMMA_BETWEEN, counters, index);
                }
                *error_found = 1;
            }
            if (index == sentence.length-1 && curr_char == ','){
                report_error(sentence.line, COMMA_NO_FOLLOWING_NUMBER, counters, index);
                *error_found = 1;
            }
        }
    }

static void check_if_after_comma(const char *curr_char, short int *is_after_comma) {
    if (*curr_char == ',') {
        *is_after_comma = 1;
    } else if (*curr_char != ' ' && *curr_char != '\t') {
        *is_after_comma = 0;
    }
}
static void update_loop_data_inspection_variables(line sentence, char* curr_char, char* last_char, int* index){
    if (*index < sentence.length) {
        (*index)++;
        *last_char = *curr_char;
        *curr_char = *(sentence.line + (*index));
    }
}
static void detect_extern_entry_errors(line* sentence, line_marks_index indexes, line_marks_counter* counters, short int* error_found) {
    if (!strcmp(sentence->data_parts.order, "entry") || !strcmp(sentence->data_parts.order, "extern")) {
        int order_end_index = indexes.dot_index + (int) strlen(sentence->data_parts.order);
        char* i = sentence->line + indexes.data_index;
        short int data_ended = 0;
        if (indexes.data_index == -1) {/*label wasn't found*/
            report_error(sentence->line, EXTERN_ENTRY_NO_LABEL, counters, order_end_index);
            *error_found = 1;
        } else {
            inspect_label(sentence, indexes, counters, indexes.data_index, error_found);
        }
        check_pre_order_chars(*sentence, indexes, counters, error_found);
        while (*i) {
            if (!data_ended && (*i == ' ' || *i == '\t')){
                data_ended = 1;
            }
            if (data_ended == 1 && *i != ' ' && *i != '\t') {
                report_error(sentence->line, UNEXPECTED_CHARACTER, counters, *i, i - sentence->line);
                *error_found = 1;
            }
            i++;
        }
    }
}
static void check_pre_order_chars(line sentence, line_marks_index indexes, line_marks_counter* counters, short int* error_found){
    short int error_code;
    if (!strcmp(sentence.data_parts.order, "data")){
        error_code = CHARS_BEFORE_DATA_ORDER;
    }
    if (!strcmp(sentence.data_parts.order, "string")){
        error_code = CHARS_BEFORE_STRING_ORDER;
    }
    if (!strcmp(sentence.data_parts.order, "extern") || !strcmp(sentence.data_parts.order, "entry")){
        error_code = CHARS_BEFORE_EXTERN_OR_ENTRY;
    }
    /*we are  here only if the first dot is a proper order, so the dot index surely is the index of the order*/
    /*check if there are chars between the label and the order*/
    if (*(sentence.label.name) != '\0' && indexes.colon_index < indexes.dot_index){
        int index_of_word_after_label = find_next_word(sentence.line, indexes.colon_index+1);
        if (*(sentence.line + index_of_word_after_label) != '.') {
            report_error(sentence.line, error_code, counters, index_of_word_after_label);
            *error_found = 1;
        }
    } else if (*(sentence.line + indexes.first_char_index) != '.'){
        report_error(sentence.line, error_code, counters, indexes.first_char_index);
        *error_found = 1;
    }
}
/*~~code inspection section~~*/
static void inspect_code_line(line sentence, line_marks_index indexes, line_marks_counter* counters, short int* error_found){

}
/*~~label inspection section~~*/
static void inspect_label(line* sentence, line_marks_index indexes, line_marks_counter* counters, int start_index, short int* error_found){
    check_label_length(sentence, indexes, counters, start_index, error_found);
    check_label_syntax(*sentence, indexes, counters, start_index, error_found);
}
static void check_label_length(line* sentence, line_marks_index indexes, line_marks_counter* counters, int start_index, short int* error_found){
    int label_length;
    int label_index;
    if (*(sentence->label.name) != '\0' && start_index < indexes.colon_index) {/*if that's a label definition*/
        label_length = indexes.colon_index - indexes.first_char_index;
        label_index = indexes.first_char_index;
    } else{
        char* i = sentence->line + indexes.data_index;
        while (*i != ' ' && *i != '\t' && *i){
            i++;
        }
        label_length = i - sentence->line - indexes.data_index;
        label_index = indexes.data_index;
    }
    if (label_length > 31){
        report_error(sentence->line, LABEL_TOO_LONG, counters, label_index);
        *error_found = 1;
    }
}
static void check_label_syntax(line sentence, line_marks_index indexes, line_marks_counter* counters, int start_index, short int* error_found){
    char curr_char = *(sentence.line + start_index);
    if (curr_char >= '0' && curr_char <= '9'){
        report_error(sentence.line, LABEL_STARTS_WITH_NUMBER, counters, start_index);
        *error_found = 1;
    }else if (curr_char < 'A' || (curr_char > 'Z' && curr_char < 'a') || curr_char > 'z'){
        report_error(sentence.line, LABEL_DOESNT_START_WITH_LETTER, counters, start_index);
        *error_found = 1;
    }
    while (curr_char != ' ' && curr_char != '\t' && curr_char != '\0' && curr_char != ':'){
        if (curr_char < '0' || (curr_char > '9' && curr_char < 'A') || (curr_char > 'Z' && curr_char < 'a') || curr_char > 'z'){
            report_error(sentence.line, ILLEGAL_LABEL_SYNTAX, counters, start_index);
            *error_found = 1;
        }
        start_index++;
        curr_char = *(sentence.line + start_index);
    }
    if (*(sentence.label.name) != '\0' && start_index < indexes.colon_index &&
        (curr_char == ' ' || curr_char == '\t')) {/*if there is space in a label definition*/
        report_error(sentence.line, SPACE_IN_LABEL, counters, start_index);
        *error_found = 1;
    }
}