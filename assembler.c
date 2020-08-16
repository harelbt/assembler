#include <stdlib.h>
#include "first pass.h"
#include "second_pass.h"
int main (int argc, char* argv[]){
    char error_found = FALSE;
    int i = 1;
    FILE* temp_machine_code;
    FILE* machine_code;
    FILE *source;
    char* file_name_without_type;
    /*checks if operators supplied*/
    if (argc == 1) {
        /*prints this error to stderr with exit code 1*/
        stop(EXIT_FAILURE, "no assembly code was supplied.\nassembly codes can be given as command line arguments");
    }
    /*assembles each supplied file*/
    while (i < argc) {
        line_counters counters;
        symbol* symbol_table = allocate_memory(1, SYMBOL);
        *(symbol_table->name) = '\0';
        symbol_table->next = NULL;
        /*opening file*/
        source = open_file(*(argv + i), "r");/*this custom function can handle malloc failure*/
        file_name_without_type = get_file_name_without_type(*(argv + i));
        machine_code = open_machine_code(file_name_without_type, "w+");
        temp_machine_code = open_file("temp.TXT", "w+");
        first_pass(source, *(argv + i), temp_machine_code, symbol_table, &counters, &error_found);
        print_words_count(machine_code, &counters);
        unite_temp_with_machine_code(temp_machine_code, machine_code);
        second_pass(machine_code, symbol_table, source, &counters, &error_found, file_name_without_type);
        error_found = FALSE;/*resets error flag for the next file*/
        i++;
    }
    return 0;
}