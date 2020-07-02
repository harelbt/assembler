#include "first pass.h"
void first_pass(char* file){
    FILE* filep = open_file(file,"r");
    line* sentence = allocate_arr_memory(1,"line");
    while (!read_line(filep, sentence)) {
        parse(sentence);
        /*error_check(line);*/
        /*RECOGNIZE SENTENCE TYPE*/
        /*HANDLE SYMBOL*/
        /*HANDLE (OPERATORS AND OPERANDS) OR DATA*/
    }
}
int read_line(FILE* file, line* sentence){
    sentence->line = get_line_dynamic(file);
        if (!strcmp(sentence->line, ""))
            return EOF;
        puts(sentence -> line);
    return 0;
}
short int parse (line* sentence){
    comment_check(sentence);

}
short int report_error(line* sentence, short int error_code){

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
void comment_check(line* sentence){
    int semicolon_index = find_semicolon(sentence);
    if (!semicolon_index){
        sentence->is_comment = 1;
    }
    if (semicolon_index > 0){

    }
}