#include <stdlib.h>
#include "in out tools.h"
#include "first pass.h"
#include "second_pass.h"
static void free_symbol_table(symbol* symbol_table, FILE* source);
int main (int argc, char* argv[]){
    char error_found = FALSE;
    char is_entry = FALSE;
    char is_external = FALSE;
    int i = 1;
    FILE* machine_code;
    FILE *source;
    char* name_without_type;
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
        name_without_type = get_file_name_without_type(*(argv + i));
        machine_code = open_machine_code(name_without_type);
        first_pass(source, *(argv + i), machine_code, symbol_table, &counters, &error_found);
        second_pass(machine_code, symbol_table, source, &counters, &error_found, name_without_type,
                    &is_entry, &is_external);
        fclose(machine_code);
        remove_unnecessary_files(name_without_type, &error_found, &is_external, &is_entry);
            free_symbol_table(symbol_table, source);
        error_found = FALSE;/*resets error flag for the next file*/
        i++;
    }
    return 0;
}
static void free_symbol_table(symbol* symbol_table, FILE* source){
    while (symbol_table != NULL){
        symbol* next_pointer = symbol_table->next;
            free(symbol_table);
            symbol_table = NULL;
        symbol_table = next_pointer;
    }
}