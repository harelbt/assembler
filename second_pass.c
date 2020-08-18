#include <stdio.h>
#include <stdlib.h>
#include "in out tools.h"
#include "second_pass.h"
#include "errors.h"
#include "symbol table.h"
#include <string.h>
#include "translator.h"
#include <ctype.h>
void second_pass(FILE * machine_code, FILE* temp_machine_code, symbol * symbol_table, FILE * input_file, line_counters* counters, char* error_found,
                char* file_name_without_type){
    FILE* externals_file = create_ext_files(file_name_without_type);
    FILE* entries_file = create_ent_files(file_name_without_type);
    char is_entry = FALSE;
    char is_external = FALSE;
    code_symbols(machine_code, temp_machine_code, symbol_table, counters, externals_file, &is_external, error_found);
    if (*error_found == FALSE) {
        is_entry = add_entries(input_file, symbol_table, entries_file, counters, error_found);
    }
    close_files(externals_file, entries_file, machine_code);
    free_symbol_table(symbol_table);
    remove_unnecessary_files(file_name_without_type, error_found, is_external, is_entry);
}
/**/
static void code_symbols(FILE* machine_code, FILE* temp_machine_code, symbol* symbol_table, line_counters* counters, FILE* externals_file, char* is_external, char* error_found) {
    address label_address;
    symbol *symbol_to_code;
    char *curr_line;
    char curr_address[NUMBER_ALLOWED_LENGTH];
    int curr_address_iterator = 0;
    int curr_char;
    int line_length;
    char *i;
    counters->line_number = 1;
    fseek(temp_machine_code, START, SEEK_SET);
    while ((curr_char = fgetc(temp_machine_code)) != EOF) {
        if (curr_char >= '0' && curr_char <= '9') {
            *(curr_address + curr_address_iterator) = (char) curr_char;
            curr_address_iterator++;
        }
        if (curr_char == ' ' || curr_char == '\t' || curr_char == '\n' || curr_char == '\r') {
            *(curr_address + curr_address_iterator) = '\0';
            curr_address_iterator = 0;
        }
        if (curr_char == '?') {
            label_address.label_address_binary = 0;
            curr_line = get_line_dynamic(temp_machine_code, &line_length);
            i = curr_line;
            if (*(i) == '&') {
                i++;
                code_jump(machine_code, symbol_table, i, counters, &label_address, curr_address, error_found);
            } else {
                code_label_address(machine_code, externals_file, symbol_table, i, counters, &label_address, curr_address, error_found, is_external);
            }
            if (strcmp(curr_line, "") != 0){
                free(curr_line);
            }
        } else {
            fputc(curr_char, machine_code);
        }
    }
}
static char add_entries(FILE* input_file, symbol* symbol_table, FILE* entries_file, line_counters* counters, char* error_found){
    char* line;
    char* entry;
    symbol* entry_symbol;
    int line_length = 0;
    char is_entry = FALSE;
    fseek(input_file, START, SEEK_SET);
    counters->line_number = 0;
    while (strcmp(line = get_line_dynamic(input_file, &line_length), "") != 0){
        counters->line_number++;
        entry = get_entry(line);
        if (strcmp(entry, "") != 0){
            entry_symbol = get_symbol(symbol_table, entry);
            if (entry_symbol == NULL){
                report_error(line, ENTRY_NOT_EXIST, counters);
                *error_found = TRUE;
            } else{
                is_entry = TRUE;
                print_entry(entries_file, entry_symbol, counters->IC);
            }
        }
        free(line);
        line = NULL;
        if (strcmp(entry, "") != 0) {
            free(entry);
            entry = NULL;
        }
    }

    return is_entry;
}
static char* get_entry(char* line){
    char* i = line;
    char entry_check[ENTRY_ORDER_LENGTH] = {'\0'};
    char* label = allocate_memory(LABEL_MAX_LENGTH, CHAR);
    int index = 0;
    *(label) = '\0';
    //*(entry_check) = '\0';
    while (*i && *i != '\"' && *i != ',' && *i != ';'){
        if (*i == 'e'){
            strncpy(entry_check, i, ENTRY_ORDER_LENGTH - 1);
            if (!strcmp(entry_check, "entry")){
                i += ENTRY_ORDER_LENGTH - 1;
                while (*i == ' ' || *i == '\t'){
                    i++;
                }
                strncpy(label, i, LABEL_MAX_LENGTH - 1);
                while (*(label + index)){
                    if (*(label + index) == '\t' || *(label + index) == ' '){
                        *(label + index) = '\0';
                    }
                    index++;
                }
                return label;
            }
        }
        i++;
    }
    free(label);
    return "";
}

static void close_files(FILE *externals_file, FILE *entries_file, FILE *machine_code) {
    fclose(externals_file);
    fclose(entries_file);
    fclose(machine_code);
}