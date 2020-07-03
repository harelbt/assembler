#include "assembler data types.h"
#include "helpfunctions.h"
void insert_symbol(symbol to_enter, symbol* symbol_table, int *symbol_tabel_length){
    *symbol_tabel_length++;
    if (*symbol_tabel_length == 1){
        symbol_table = allocate_arr_memory(*symbol_tabel_length,"symbol");
    } else
        symbol_table = realloc_arr_memory(symbol_table, *symbol_tabel_length, "symbol");
    symbol_table[*symbol_tabel_length-1] = to_enter;
}
static void create_symbol(symbol* to_initialize, char* name, char* address, char sentence_type, char extern_or_entry){
    strcpy(to_initialize->name, name);
    strcpy(to_initialize->address, address);
    to_initialize->extern_or_entry = extern_or_entry;
    to_initialize->sentence_type = sentence_type;
}