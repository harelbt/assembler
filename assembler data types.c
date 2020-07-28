#include <stdlib.h>
#include "assembler data types.h"
/*#include "helpfunctions.h"
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
}*/
void initialize_line_tools(line* sentence, line_marks_counter* counters, line_marks_index* indexes){
    initialize_counters(counters);
    initialize_indexes(indexes);
    initialize_line(sentence);
}
void initialize_counters(line_marks_counter* counters){
    counters->number_of_hashmarks = 0;
    counters->number_of_quotation_marks = 0;
    counters->number_of_dots = 0;
    counters->number_of_operators = 0;
    counters->number_of_colons = 0;
    counters->number_of_registers = 0;
    counters->line_number = 0;
    counters->number_of_operands = 0;
}
void initialize_indexes(line_marks_index* indexes){
    indexes->first_hash_mark_index = -1;
    indexes->first_char_index = -1;
    indexes->first_register_index = -1;
    indexes->first_quotation_mark_index = -1;
    indexes->second_quotation_mark_index = -1;
    indexes->second_hash_mark_index = -1;
    indexes->second_register_index = -1;
    indexes->dot_index = -1;
    indexes->colon_index = -1;
    indexes->semicolon_index = -1;
    indexes->operator_index = -1;
    indexes->first_operand_index = -1;
    indexes->second_operand_index = -1;
    indexes->data_index = -1;
}
void initialize_line(line* sentence){
    sentence->flags.is_empty_line = FALSE;
    sentence->flags.is_data = FALSE;
    sentence->flags.is_code = FALSE;
    sentence->flags.is_comment = FALSE;
    sentence->code_parts.operator_parts.opcode = -1;
    sentence->code_parts.operator_parts.function = -1;
    sentence->data_parts.data = NULL;
    *(sentence->data_parts.order) = '\0';
    sentence->label.address = NULL;
    *(sentence->label.name) = '\0';
}
