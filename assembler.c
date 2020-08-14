#include <stdlib.h>
#include "in out tools.h"
#include "first pass.h"
#include "second_pass.h"
int main (int argc, char* argv[]){
    char error_found = FALSE;
    char is_entry = FALSE;
    char is_external = FALSE;
    int i = 1;
    symbol* symbol_table = allocate_arr_memory(1, SYMBOL);
    FILE* machine_code;
    FILE *filep;
    char* name_without_type;
    /*checks if operators supplied*/
    if (argc == 1) {
        /*prints this error to stderr with exit code 1*/
        stop(EXIT_FAILURE, "no assembly code was supplied");
    }
    /*assembles each supplied file*/
    while (i < argc) {
        line_counters counters;
        /*opening file*/
        filep = open_file(*(argv+i), "r");/*this custom function can handle malloc failure*/
        name_without_type = get_file_name_without_type(*(argv + i));
        machine_code = open_machine_code(name_without_type);
        first_pass(filep, *(argv + i), machine_code, symbol_table, &counters, &error_found);
        second_pass(machine_code, symbol_table, filep, &counters, &error_found, name_without_type,
                &is_entry, &is_external);
        fclose(machine_code);
        if (error_found == TRUE) {
            remove_output_files(name_without_type);
        } else{
            if (is_external == FALSE){
                remove_ext_file(name_without_type);
            }
            if (is_entry == FALSE){
                remove_ent_file(name_without_type);
            }
        }
        error_found = FALSE;/*resets error flag for the next file*/
        i++;
    }
    return 0;
}
