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
        parse(sentence, &error_list);
        /*RECOGNIZE SENTENCE TYPE*/
        /*HANDLE SYMBOL*/
        /*HANDLE (OPERATORS AND OPERANDS) OR DATA*/
    }
    if (error_list.error != NULL) {
        output_errors(&error_list);
    }
}
int read_line(FILE* file, line* sentence){
    sentence->line = get_line_dynamic(file);
        if (!strcmp(sentence->line, ""))
            return EOF;
    return 0;
}
short int parse (line* sentence, error* error_list){
    comment_check(sentence, error_list);

}

int find_semicolon(line* sentence){
    int i = 0;
    while (sentence->line[i]){
        if (sentence->line[i] == ';')
            return i;
        i++;
    }
    return -1;
}
void comment_check(line* sentence, error* error_list){
    int semicolon_index = find_semicolon(sentence);
    if (!semicolon_index){
        sentence->is_comment = 1;
    }
    if (semicolon_index > 0){
        report_error(sentence, UNEXPECTED_SEMICOLON, error_list);
    }
}