#include "first pass.h"
int first_pass(char* file){
    FILE* filep = open_file(file,"r");
    while (filep != EOF) {
        read_line(filep);
    }
}
void read_line(FILE* file){
    char* line = get_line_dynamic(file);
    /*ERROR HANDLING HERE*/
    /*RECOGNIZE SENTENCE TYPE*/
    /*HANDLE SYMBOL*/
    /*HANDLE (OPERATORS AND OPERANDS) OR DATA*/
}