#include <stdlib.h>
#include "symbol table.h"
#include "helpfunctions.h"
#include "errors.h"
symbol* insert_symbol(symbol* to_insert, symbol* symbol_table, char* is_first_symbol, line_marks_counter* counters, char* error_found) {
    symbol *curr_pointer = symbol_table;
    symbol *prev_pointer = NULL;
    symbol* new_symbol;
    if (*is_first_symbol == TRUE){
        to_insert->next = NULL;
        symbol_copy(symbol_table, to_insert);
        *is_first_symbol = FALSE;
        return symbol_table;
    }
    new_symbol = allocate_arr_memory(1, "symbol");
    symbol_copy(new_symbol, to_insert);
    while (curr_pointer != NULL) {
        if (*curr_pointer->name >= *new_symbol->name) {
            if (!strcmp(curr_pointer->name, new_symbol->name)){
                report_error(new_symbol->name, SECOND_LABEL_DEFINITION, counters);
                *error_found = TRUE;
                return symbol_table;
            }
            if (prev_pointer == NULL) {
                new_symbol->next = symbol_table;
                return new_symbol;
            } else {
                new_symbol->next = curr_pointer;
                prev_pointer->next = new_symbol;
                return symbol_table;
            }
        }
        prev_pointer = curr_pointer;
        curr_pointer = curr_pointer->next;
    }
    if (curr_pointer == NULL) {
        new_symbol->next = NULL;
        prev_pointer->next = new_symbol;
    }
    return symbol_table;
}
static void symbol_copy(symbol* dest, symbol* source){
    strcpy(dest->name, source->name);
    dest->address = source->address;
    dest->extern_or_entry = source->extern_or_entry;
    dest->sentence_type = source->sentence_type;
    dest->next = source->next;
}