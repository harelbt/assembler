#include "assembler data.h"
#include "helpfunctions.h"
void insert_symbol(symbol to_enter, symbol* symbol_table, int *symbol_tabel_length){
    *symbol_tabel_length++;
    if (*symbol_tabel_length == 1){
        symbol_table = allocate_arr_memory(*symbol_tabel_length,"symbol");
    } else
        symbol_table = realloc_arr_memory(symbol_table, *symbol_tabel_length, "symbol");
    symbol_table[*symbol_tabel_length-1] = to_enter;
}
symbol create_symbol(char* name, char* address, char sentence_type, char extern_or_entry){
    symbol* new_symbol = allocate_arr_memory(1, "symbol");
    strcpy(new_symbol->name, name);
    strcpy(new_symbol->address, address);
    new_symbol->extern_or_entry = extern_or_entry;
    new_symbol->sentence_type = sentence_type;
    return *new_symbol;
}