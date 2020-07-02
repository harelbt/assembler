#include "first pass.h"
void first_pass(char* file){
    FILE* filep = open_file(file,"r");
    line* sentence = NULL;
    while (!read_line(filep, sentence)) {
        short int is_comment = 0;
        is_comment = parse(line,);
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
short int parse (char* label, char* operator, char* first_operand, char* second_operand){

}
short int error_check(char* line){

}