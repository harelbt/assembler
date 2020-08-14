#include <time.h>
#include <string.h>
#include "assembler.h"
#include "second_pass.h"
int main (int argc, char* argv[]){
    char error_found = FALSE;
    char is_entry = FALSE;
    char is_external = FALSE;
    int i = 1;
    symbol* symbol_table;
    FILE* machine_code;
    FILE *filep;
    line_counters counters;
    char* name_without_type;
    /*checks if operators supplied*/
    if (argc == 1) {
        /*prints this error to stderr with exit code 1*/
        stop(EXIT_FAILURE, "no assembly code was supplied");
    }
    /*assembles each supplied file*/
    while (i < argc) {
        /*opening file*/
        filep = open_file(*(argv+i), "r");/*this custom function can handle malloc failure*/
        name_without_type = get_file_name_without_type(*(argv + i));
        machine_code = open_machine_code(name_without_type);
        symbol_table = first_pass(filep, *(argv + i), machine_code, &counters, &error_found);
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
static char* get_file_name_without_type(char* file_name){
    unsigned int length = strlen(file_name);
    char* i = file_name + length;
    while (*i != '.'){
        i--;
    }
    *i = '\0';
    return file_name;
}
static void remove_output_files(char* file_name){
    remove_ob_file(file_name);
    remove_ent_file(file_name);
    remove_ext_file(file_name);
}
static void remove_ob_file(char* file_name){
    char to_remove [strlen(file_name) + TYPE_MAX_LENGTH];
    strcpy(to_remove, file_name);
    strcat(to_remove, ".ob");
    remove(to_remove);
}
void remove_ent_file(char* file_name){
    char to_remove [strlen(file_name) + TYPE_MAX_LENGTH];
    strcpy(to_remove, file_name);
    strcat(to_remove, ".ext");
    remove(to_remove);
}
static void remove_ext_file(char* file_name){
    char to_remove [strlen(file_name) + TYPE_MAX_LENGTH];
    strcpy(to_remove, file_name);
    strcat(to_remove, ".ent");
    remove(to_remove);
}
static FILE* open_machine_code(char* file_name){
    char to_open[strlen(file_name) + TYPE_MAX_LENGTH];
    FILE* machine_code;
    strcpy(to_open, file_name);
    strcat(to_open, ".ob");
    machine_code = open_file(to_open, "a+");
    return machine_code;
}