#include "second pass.h"
#include "errors.h"
void second_pass(FILE* machine_code,FILE* source, symbol* symbol_table){
    code_symbols(machine_code, symbol_table);
    add_entries();
    prepare_output_files();
}
static void code_symbols(FILE* machine_code, symbol* symbol_table){
    char* symbol_to_code[LABEL_MAX_LENGTH];
    int curr_char;
    fseek(machine_code, 0, SEEK_SET);
    while ((curr_char = fgetc(machine_code)) != EOF){
    }
}
static void add_entries(){}
static void prepare_output_files(){}