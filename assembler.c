#include <time.h>
#include "assembler.h"
int main (int argc, char* argv[]){
    char error_found = FALSE;
    int i = 1;
    symbol* symbol_table;
    symbol* symbol_addresses_table[NUM_OF_ENGLISH_CHARS];
    FILE* machine_code;
    FILE *filep;
    clock_t start = clock();
    clock_t end;
    double runtime;
    /*checks if operators supplied*/
    if (argc == 1) {
        /*prints this error to stderr with exit code 1*/
        stop(EXIT_FAILURE, "no assembly code was supplied");
    }
    /*assembles each supplied file*/
    while (i < argc) {
        /*opening file*/
        filep = open_file(*(argv+i), "r");/*this custom function can handle malloc failure*/
        machine_code = open_machine_code(machine_code, *(argv+i));
        symbol_table = first_pass(filep, machine_code, symbol_addresses_table, &error_found);

        if (error_found == TRUE) {
        }
        fclose(machine_code);
        error_found = FALSE;/*resets error flag for the next file*/
        i++;
    }
    end = clock();
    runtime = (double)(end - start)/CLOCKS_PER_SEC;
    printf("%f", runtime);
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