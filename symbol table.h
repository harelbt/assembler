#ifndef ASSEMBLER_SYMBOL_TABLE_H
#define ASSEMBLER_SYMBOL_TABLE_H
#include "assembler data types.h"
#define CAPITAL_Z_TO_A_GAP 6
symbol* insert_symbol(symbol* to_insert, symbol* symbol_table, char* is_first_symbol, line_counters* counters, char* error_found);
void update_symbol_address(line* sentence, line_counters counters);
void* get_symbol(symbol* symbol_table, char* symbol_name, symbol** symbol_table_addresses);
static void symbol_copy(symbol* dest, symbol* source);
#endif //ASSEMBLER_SYMBOL_TABLE_H
