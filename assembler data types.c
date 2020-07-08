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
void initialize_operator_variables(operator* op_variables){
    op_variables->recognized_function = -1;
    op_variables->recognized_opcode = -1;
}
void initialize_counters(line_marks_counter* counters){
    counters->number_of_hashmarks = 0;
    counters->number_of_quotation_mark = 0;
    counters->number_of_dots = 0;
    counters->number_of_operators = 0;
    counters->number_of_colons = 0;
    counters->number_of_registers = 0;
}
void initialize_indexes(line_marks_index* indexes){
    indexes->first_hash_mark_index = 0;
    indexes->first_char_index = 0;
    indexes->first_register_index = 0;
    indexes->first_quotation_mark_index = 0;
    indexes->second_quotation_mark_index = 0;
    indexes->second_hash_mark_index = 0;
    indexes->second_register_index = 0;
    indexes->dot_index = 0;
    indexes->colon_index = 0;
    indexes->semicolon_index = 0;
}
void initialize_line(line* sentence){
    sentence->line_number = 0;
    sentence->flags.is_empty_line = FALSE;
    sentence->flags.is_order = FALSE;
    sentence->flags.is_instruction = FALSE;
    sentence->flags.is_comment = FALSE;
    sentence->flags.is_label = FALSE;
    sentence->code_parts.opcode = -1;
    sentence->code_parts.function = -1;
}