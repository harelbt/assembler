#include <stdlib.h>/*for EXIT_FAILURE macro*/
#include "first pass.h"
#include "second_pass.h"
#include "errors.h"
int main (int argc, char* argv[]){
    /*errors_flag*/
    char error_found = FALSE;
    /*files*/
    FILE* temp_machine_code;/*temporary file for first pass translation. without the first line of the words count(ic-100 dc-100)*/
    FILE* machine_code;/*the .ob file*/
    FILE *source;/*input file*/
    /*name without type for opening and removing different types of files(.ob, .ent, .ext)*/
    char* file_name_without_type;
    /**/
    int i = 1;
    /*checks if operators isn't supplied*/
    if (argc == 1) {
        /*prints this error to stderr with exit code 1*/
        stop(EXIT_FAILURE, "no assembly code was supplied.\nassembly codes can be given as command line arguments");
    }
    /*assembles each supplied file, all the program executes through this loop*/
    while (i < argc) {
        /*a type that contains all the necessary counters for the program, including IC, DC*/
        line_counters counters;
        /*symbol_table_head*/
        symbol* symbol_table = allocate_memory(ONE_UNIT, SYMBOL);/*this custom function can handle malloc failure*/
        *(symbol_table->name) = '\0';
        symbol_table->next = NULL;
        /*OPENING FILES*/
        source = open_file(*(argv + i), "r");/*this custom function can handle fopen failure*/
        file_name_without_type = get_file_name_without_type(*(argv + i));
        machine_code = open_machine_code(file_name_without_type, "w+");
        temp_machine_code = open_file("temp.TXT", "w+");
        /*ASSEMBLING*/
        first_pass(source, *(argv + i), temp_machine_code, symbol_table, &counters, &error_found);
        if (error_found == FALSE) {
            print_words_count(machine_code, &counters);
            second_pass(machine_code, temp_machine_code, symbol_table, source, &counters, &error_found,
                        file_name_without_type);
        } else{
            fclose(machine_code);
            remove_ob_file(file_name_without_type);
        }
        fclose(temp_machine_code);
        remove_temp_file();
        if (error_found == TRUE){
            print_errors_summary(file_name_without_type, counters.error_number);
        }
        /*resets error flag for the next file*/
        error_found = FALSE;
        i++;
    }
    return 0;
}