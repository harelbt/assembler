#include "assembler.h"
int main (int argc, char* argv[]){
    char error_found = FALSE;
    int i = 1;
    symbol* symbol_table;
    symbol symbol_indexes_table[NUM_OF_ENGLISH_CHARS];
    FILE* machine_code;
    /*checks if operators supplied*/
    if (argc == 1) {
        /*prints this error to stderr with exit code 1*/
        stop(EXIT_FAILURE, "no assembly code was supplied");
    }
    /*assembles each supplied file*/
    while (i < argc) {
        machine_code = open_machine_code(machine_code, *(argv+i));
        symbol_table = first_pass(*(argv+i), machine_code, symbol_indexes_table, &error_found);
        if (error_found == TRUE) {
            /*second_pass();*/
        }
        fclose(machine_code);
        error_found = FALSE;/*resets error flag for the next file*/
        i++;
    }
    while (symbol_table){
        printf("%s %d\n", symbol_table->name, symbol_table->address);
        symbol_table = symbol_table->next;
    }
    return 0;
}
FILE * open_machine_code(FILE* machine_code, char* file_name){
    unsigned int file_name_length = strlen(file_name);
    char output_file_name [file_name_length];
    strncpy(output_file_name, file_name, file_name_length - 3);
    *(output_file_name + file_name_length - 3) = 'o';
    *(output_file_name + file_name_length - 2) = 'b';
    *(output_file_name + file_name_length - 1) = '\0';
    machine_code = open_file(output_file_name, "a+");
    return machine_code;
}