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
    empty_line_check(sentence);
    if (sentence->flags.is_empty_line == FALSE) {
        comment_check(sentence, error_list);
        if (sentence->flags.is_comment == FALSE){

        }
    }
}
void find_signs(line* sentence, line_marks_index* indexes, error* error_list){
    int i = 0;
    short int colon_found = 0;
    short int semicolon_found = 0;
    short int dot_found = 0;
    short int number_of_hash_marks = 0;
    short int number_of_quotation_marks = 0;
    short int number_of_registers = 0;
    while (sentence->line[i]){
        switch (sentence->line[i]) {
            case ':':{
                if (!colon_found) {
                    indexes->colon_index = i;
                    colon_found = 1;
                }
                break;
            }
            case ';':{
                if (!semicolon_found) {
                    indexes->semicolon_index = i;
                    semicolon_found = 1;
                }
                break;
            }
            case '.':{
                if (!dot_found) {
                    indexes->dot_index = i;
                    dot_found = 1;
                }
                break;
            }
            case '#':{
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
                }
            }
            case 'r':{}
            case '\"':{}
        }
        i++;
    }
}
void comment_check(line* sentence, error* error_list){
    int semicolon_index = find_semicolon(sentence);
    int first_char_index = find_first_char(sentence);
    if (semicolon_index == first_char_index && semicolon_index >= 0){
        sentence->flags.is_comment = TRUE;
    }
    if (semicolon_index > first_char_index && semicolon_index > 0 && first_char_index >= 0){
        report_error(sentence, UNEXPECTED_SEMICOLON, error_list);
    }
}
void empty_line_check (line* sentence){
    if (find_first_char(sentence) == -1){
        sentence->flags.is_empty_line = TRUE;
    } else
        sentence->flags.is_empty_line = FALSE;
}
