#ifndef ASSEMBLER_SYMBOL_TABLE_H
#define ASSEMBLER_SYMBOL_TABLE_H
#include "assembler data types.h"
symbol* insert_symbol(symbol* to_insert, symbol* symbol_table, char* is_first_symbol, line_marks_counter* counters, char* error_found);
void update_symbol_address(line* sentence, line_marks_counter counters);
static void symbol_copy(symbol* dest, symbol* source);
#endif //ASSEMBLER_SYMBOL_TABLE_H
