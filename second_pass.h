#ifndef ASSEMBLER_SECOND_PASS_H
#define ASSEMBLER_SECOND_PASS_H
#include <stdio.h>
#include "assembler data types.h"
#define STRING_LEN 50
#define ENTRY_ORDER_LENGTH 6
#define ADDRESS_PRINT_LENGTH 7
void second_pass(FILE * first_pass_file, symbol * symbol_table,FILE * input_file,
        line_counters* counters, char* error_found, char* file_name_without_type, char* is_entry, char* is_external);
static FILE* create_ent_files(char* name_without_type);
static FILE* create_ext_files(char* name_without_type);
void writeHexa(unsigned int label_address,FILE * machine_code);
static void code_symbols(FILE* machine_code, symbol* symbol_table, FILE* externals_file, char* is_external);
static char add_entries(FILE* input_file, symbol* symbol_table, FILE* entries_file, line_counters* counters, char* error_found);
static char* get_entry(char* line);
static void print_entry_extern(FILE* file, symbol* entry_extern);
static int get_address_length(int address);
#endif //ASSEMBLER_SECOND_PASS_H
