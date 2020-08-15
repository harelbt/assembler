#include <stdlib.h>
#include <string.h>
#include "symbol table.h"
#include "in out tools.h"
symbol* insert_symbol(symbol* to_insert, symbol* symbol_table, char* is_first_symbol, line_counters* counters, char* error_found) {
    symbol *curr_pointer = symbol_table;
    symbol* new_symbol;
    if (*is_first_symbol == TRUE){
        to_insert->next = NULL;
        symbol_copy(symbol_table, to_insert);
        *is_first_symbol = FALSE;
        return symbol_table;
    }
    new_symbol = allocate_memory(1, SYMBOL);
    symbol_copy(new_symbol, to_insert);
    while (curr_pointer->next != NULL) {
        curr_pointer = curr_pointer->next;
    }
    curr_pointer->next = new_symbol;
    new_symbol->next = NULL;
    return symbol_table;
}
void update_symbol_address(line* sentence, line_counters counters){
    if (sentence->flags.is_code == TRUE){
        if (*sentence->label.name != '\0'){
            int distance_from_last_IC = counters.IC - counters.last_instruction_address;
            sentence->label.address = counters.IC - (distance_from_last_IC);
        }
    } else{
        if (*sentence->label.name != '\0'){
            if (strcmp(sentence->data_parts.order, "extern") != 0) {
                int distance_from_last_DC = counters.DC - counters.last_data_address;
                sentence->label.address = counters.DC - (distance_from_last_DC);
            } else{
                sentence->label.address = 1;
            }
        }
    }
}
void* get_symbol(symbol* symbol_table, char* symbol_name){
    symbol* symbol_pointer = symbol_table;
    while (symbol_pointer){
        if (!strcmp(symbol_name, symbol_pointer->name)){
            return symbol_pointer;
        }
        symbol_pointer = symbol_pointer->next;
    }
    return NULL;
}
static void symbol_copy(symbol* dest, symbol* source){
    strcpy(dest->name, source->name);
    dest->address = source->address;
    dest->extern_or_entry = source->extern_or_entry;
    dest->next = source->next;
}