#include "first pass.h"
void first_pass(char* file){
    FILE* filep = open_file(file,"r");
    /*reading the file and handles everything*/
    while (!read_line(filep)) {
    }
}
int read_line(FILE* file){
    line* sentence;
    short int error_found = 0;
    sentence->line = get_line_dynamic(file);
        if (!strcmp(line, ""))
            return EOF;
        puts(line);
        is_comment = parse(line,);
        /*error_check(line);*/
        /*RECOGNIZE SENTENCE TYPE*/
        /*HANDLE SYMBOL*/
        /*HANDLE (OPERATORS AND OPERANDS) OR DATA*/
        free(line);
    return 0;
}
short int parse (char* label, char* operator, char* first_operand, char* second_operand){

}
short int error_check(char* line){

}