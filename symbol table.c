#include <stdlib.h>
#include "symbol table.h"
#include "helpfunctions.h"
#include "errors.h"
symbol* insert_symbol(symbol* to_insert, symbol* symbol_table, char* is_first_symbol, line_counters* counters, char* error_found) {
    symbol *curr_pointer = symbol_table;
    symbol *prev_pointer = NULL;
    symbol* new_symbol;
    if (*is_first_symbol == TRUE){
        to_insert->next = NULL;
        symbol_copy(symbol_table, to_insert);
        *is_first_symbol = FALSE;
        return symbol_table;
    }
    new_symbol = allocate_arr_memory(1, SYMBOL);
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
void update_symbol_address(line* sentence, line_counters counters){
    if (sentence->flags.is_code == TRUE){
        if (*sentence->label.name != '\0'){
            sentence->label.address = counters.IC - (counters.IC - counters.last_instruction_address);
        }
    } else{
        if (*sentence->label.name != '\0'){
            if (strcmp(sentence->data_parts.order, "extern") != 0) {
                sentence->label.address = counters.DC - (counters.DC - counters.last_data_address);
            } else{
                sentence->label.address = 1;
            }
        }
    }
}
void* get_symbol(symbol* symbol_table, char* symbol_name, symbol** symbol_table_addresses){
    symbol* symbol_pointer;
    int first_potential_symbol_index;
    if (*symbol_name <= 'Z'){
        first_potential_symbol_index = *symbol_name - 'A';
    } else{
        first_potential_symbol_index = *symbol_name - 'A' - CAPITAL_Z_TO_A_GAP;
    }
    symbol_pointer = *(symbol_table_addresses + first_potential_symbol_index);
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
    dest->sentence_type = source->sentence_type;
    dest->next = source->next;
}