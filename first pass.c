#include "first pass.h"
int first_pass(char* file){
    FILE* filep = open_file(file,"r");
    /*reading the file and handles everything*/
    while (read_line(filep) != EOF) {}
}
int read_line(FILE* file){
    char* line = get_line_dynamic(file);
    if (!strcmp(line, ""))
        return EOF;
    /*ERROR HANDLING HERE*/
    /*RECOGNIZE SENTENCE TYPE*/
    /*HANDLE SYMBOL*/
    /*HANDLE (OPERATORS AND OPERANDS) OR DATA*/
}
