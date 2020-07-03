#include "first pass.h"

void first_pass(char* file){
    FILE* filep = open_file(file,"r");
    line* sentence = allocate_arr_memory(1,"line");
    error error_list;
    error_list.line = 0;
    error_list.error = NULL;
    error_list.next = NULL;
    int line_number = 1;
    while (!read_line(filep, sentence)) {
        sentence->line_number = line_number;
        line_number++;
        if (strcmp(sentence->line, "\n")) {
            parse_line(sentence, &error_list);
            /*RECOGNIZE SENTENCE TYPE*/
            /*HANDLE SYMBOL*/
            /*HANDLE (OPERATORS AND OPERANDS) OR DATA*/
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
short int parse_line (line* sentence, error* error_list){
    empty_line_check(sentence);
    if (sentence->flags.is_empty_line == FALSE) {
        comment_check(sentence, error_list);
        if (sentence->flags.is_comment == FALSE){

        }
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