#include "helpfunctions.h"
#include "first pass.h"
int main (int argc, char* argv[]){
    int IC = 100;
    int DC = 0;
    symbol* symbol_table = NULL;
    int* data_table = NULL;
    long symbol_tabel_length = 0;
    int i = 1;
    if (argc == 1) {
        stop(EXIT_FAILURE, "no assembly code was supplied");
    }
    while (i < argc) {
        first_pass(argv[i]);
        /*second_pass();*/
        i++;
    }
    return 0;
}