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
    char* operators_table[16][3] = {{"mov", "0", ""},{"cmp", "1", ""},{"add", "2", "1"},{"sub", "2", "2"},{"lea", "4", ""},
                                {"clr", "5", "1"},{"not", "5", "2"},{"inc", "5", "3"},{"dec", "5", "4"},{"jmp", "9", "1"},
                                {"bne","9", "2"},{"jsr", "9", "3"},{"red", "12", ""},{"prn", "13", ""},{"rts", "14", ""},
                                {"stop", "15", ""}};
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