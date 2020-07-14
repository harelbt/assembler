#include <stdlib.h>
#include "assembler data types.h"
#include "helpfunctions.h"
#include "first pass.h"
int main (int argc, char* argv[]){
    int IC = 100;
    int DC = 0;
    symbol* symbol_table = NULL;
    int* data_table = NULL;
    long symbol_tabel_length = 0;
    short int error_found = 0;
    int i = 1;
    if (argc == 1) {
        stop(EXIT_FAILURE, "no assembly code was supplied");
    }
    while (i < argc) {
        first_pass(argv[i], &error_found);
        if (!error_found) {
            /*second_pass();*/
        }
        error_found = 0;
        i++;
    }
    return 0;
}