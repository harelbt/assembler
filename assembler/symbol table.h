#ifndef ASSEMBLER_SYMBOL_TABLE_H
#define ASSEMBLER_SYMBOL_TABLE_H
#include "assembler data types.h"

/*FUNCTIONS DECLARATION*/
/**
 * inserts a symbol to the symbol table
 * @param to_insert symbol node
 * @param symbol_table symbol table head
 * @param is_first_symbol flag for proper functioning of the function
 * @return the new symbol table
 */
symbol* insert_symbol(symbol* to_insert, symbol* symbol_table, char* is_first_symbol);
/**
 * updates a symbol address in struct line pointer.
 * if a label doesn't exist in the line does nothing
 * @param sentence struct line pointer
 * @param counters struct line_counters pointer
 */
void update_symbol_address(line* sentence, line_counters counters);
/**
 *gets a string and a symbol table head.
 * if the string matches a symbol in the table returns the symbol*, else returns NULL
 * @param symbol_table symbol table head
 * @param symbol_name to search
 * @return the symbol* or NULL if not found
 */
void* get_symbol(symbol* symbol_table, char* symbol_name);
/**
 * given a string that starts from a symbol name, returns the symbol name
 * @param symbol_start a string that starts from a symbol name
 * @return the symbol name
 */
char* get_symbol_name(char* symbol_start);
/**
 * frees a symbol table
 * @param symbol_table to free
 */
void free_symbol_table(symbol* symbol_table);
/**
 * copies source symbol node to a destination symbol node
 * @param dest to copy to
 * @param source dest to copy from
 */
void symbol_copy(symbol* dest, symbol* source);
/**/
#endif /*ASSEMBLER_SYMBOL_TABLE_H*/
