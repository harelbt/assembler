#include "first pass.h"

void first_pass(char* file){
    FILE* filep = open_file(file,"r");
    line* sentence = allocate_arr_memory(1,"line");
    error error_list;
    error_list.error = NULL;
    int line_number = 1;
    while (!read_line(filep, sentence)) {
        sentence->line_number = line_number;
        line_number++;
        if (strcmp(sentence->line, "\n")) {
            parse_line(sentence, &error_list);
            if (error_list.error != NULL){}
        }
    }
    if (error_list.error != NULL) {
        output_errors(&error_list);
    }
    fclose(filep);
    free(sentence);
}
int read_line(FILE* file, line* sentence){
    sentence->line = get_line_dynamic(file);
        if (!strcmp(sentence->line, ""))
            return EOF;
    return 0;
}
void parse_line (line* sentence, error* error_list){
    line_marks_index indexes;
    find_signs(sentence, &indexes, error_list);
    empty_line_check(sentence, indexes);
    if (sentence->flags.is_empty_line == FALSE) {
        comment_check(sentence, error_list, indexes);
        if (sentence->flags.is_comment == FALSE){

        }
    }
}
void find_signs(line* sentence, line_marks_index* indexes, error* error_list) {
    int i = 0;
    short int colon_found = 0;
    short int semicolon_found = 0;
    short int dot_found = 0;
    short int first_char_found = 0;
    int number_of_hash_marks = 0;
    int number_of_quotation_marks = 0;
    int number_of_registers = 0;

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
void comment_check(line* sentence, error* error_list, line_marks_index indexes){
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
void empty_line_check (line* sentence, line_marks_index indexes){
    if (indexes.first_char_index == -1){
        sentence->flags.is_empty_line = TRUE;
    } else
        sentence->flags.is_empty_line = FALSE;
}
