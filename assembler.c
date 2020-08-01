#include <stdlib.h>
#include "helpfunctions.h"
#include "first pass.h"
int main (int argc, char* argv[]){
    char error_found = FALSE;
    int i = 1;
    symbol* symbol_table;
    /*checks if operators supplied*/
    if (argc == 1) {
        /*prints this error to stderr with exit code 1*/
        stop(EXIT_FAILURE, "no assembly code was supplied");
    }
    /*assembles each supplied file*/
    while (i < argc) {
        symbol_table = first_pass(*(argv+i), &error_found);
        if (error_found == TRUE) {
            /*second_pass();*/
        }
        error_found = FALSE;/*resets error flag for the next file*/
        i++;
    }
    while (symbol_table){
        printf("%s ", symbol_table->name);
        symbol_table = symbol_table->next;
    }
    return 0;
}