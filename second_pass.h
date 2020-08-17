#ifndef ASSEMBLER_SECOND_PASS_H
#define ASSEMBLER_SECOND_PASS_H
#include <stdio.h>
#include "assembler data types.h"

/*LENGTHS*/
#define ENTRY_ORDER_LENGTH 6

/*FUNCTIONS DECLARATION*/
void second_pass(FILE * machine_code, FILE* temp_machine_code, symbol * symbol_table, FILE * input_file,
                line_counters* counters, char* error_found, char* file_name_without_type);
/**/
static void code_symbols(FILE* machine_code, FILE* temp_machine_code, symbol* symbol_table, line_counters* counters, FILE* externals_file, char* is_external, char* error_found);
static char add_entries(FILE* input_file, symbol* symbol_table, FILE* entries_file, line_counters* counters, char* error_found);
static char* get_entry(char* line);
static char* get_symbol_name(char* symbol_start);
static char* get_current_address(char* line);
static void close_files(FILE *externals_file, FILE *entries_file, FILE *machine_code);
/**/
#endif //ASSEMBLER_SECOND_PASS_H
