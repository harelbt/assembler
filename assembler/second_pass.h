#ifndef ASSEMBLER_SECOND_PASS_H
#define ASSEMBLER_SECOND_PASS_H
#include <stdio.h>
#include "assembler data types.h"

/*LENGTHS*/
#define ENTRY_ORDER_LENGTH 6

/*FUNCTIONS DECLARATION*/
/**
 * completes the labels code and creates the .ext .ent files and writes to them.
 * if no entries were found removes .ent
 * if no externals were found removes .ext
 * if a label is used but not declared produces an error
 * @param machine_code to write to
 * @param temp_machine_code to copy from and translate the labels
 * @param symbol_table to translate the labels with
 * @param input_file to find entries
 * @param counters struct line_counters pointer for current line number
 * @param error_found flag to store TRUE if errors found
 * @param file_name_without_type to handle files
 */
void second_pass(FILE * machine_code, FILE* temp_machine_code, symbol * symbol_table, FILE * input_file,
                line_counters* counters, char* error_found, char* file_name_without_type);
/**
 * completes the labels code and copies the temp file to the .ob file and sets the is_external flag
 * @param machine_code to write to
 * @param temp_machine_code to copy from and translate the labels
 * @param symbol_table to translate the labels with
 * @param counters struct line_counters pointer for current line number
 * @param externals_file to write externals to
 * @param is_external sets the flag to TRUE if externals found
 * @param error_found flag to store TRUE if errors found
 */
void code_symbols(FILE* machine_code, FILE* temp_machine_code, symbol* symbol_table, line_counters* counters, FILE* externals_file, char* is_external, char* error_found);
/**
 * looks for the .entry orders end writes to .ent
 * if entry is used but label isn't declared produces an error
 * if entries found returns TRUE, else FALSE
 * @param input_file to find entries
 * @param symbol_table to serach for the given label after .entry
 * @param entries_file to write entries to
 * @param counters struct line_counters pointer for current line number
 * @param error_found flag to store TRUE if errors found
 * @return TRUE if entries found, else FALSE
 */
char add_entries(FILE* input_file, symbol* symbol_table, FILE* entries_file, line_counters* counters, char* error_found);
/**
 * gets a line and if .entry exists returns the label with no white chars before or after
 * @param line to get entry from
 * @return the label with no white chars before or after
 */
char* get_entry(char* line);
/**
 * closes .ent .ext .ob
 * @param externals_file to close
 * @param entries_file to close
 * @param machine_code to close
 */
void close_files(FILE *externals_file, FILE *entries_file, FILE *machine_code);
/**/
#endif /*ASSEMBLER_SECOND_PASS_H*/
