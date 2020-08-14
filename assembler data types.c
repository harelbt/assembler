#include <stdlib.h>
#include "assembler data types.h"
/*-explain tools or replace name-*/
void initialize_line_tools(line* sentence, line_counters* counters, line_indexes* indexes){
    initialize_counters(counters);
    initialize_indexes(indexes);
    initialize_line(sentence);
}
static void initialize_counters(line_counters* counters){
    counters->number_of_quotation_marks = 0;
    counters->number_of_dots = 0;
    counters->number_of_operators = 0;
    counters->number_of_colons = 0;
    counters->number_of_registers = 0;
    counters->number_of_operands = 0;
    counters->number_of_commas = 0;
}
static void initialize_indexes(line_indexes* indexes){
    indexes->first_char_index = NOT_FOUND;
    indexes->first_register_index = NOT_FOUND;
    indexes->first_quotation_mark_index = NOT_FOUND;
    indexes->second_quotation_mark_index = NOT_FOUND;
    indexes->second_register_index = NOT_FOUND;
    indexes->dot_index = NOT_FOUND;
    indexes->colon_index = NOT_FOUND;
    indexes->semicolon_index = NOT_FOUND;
    indexes->operator_index = NOT_FOUND;
    indexes->first_operand_index = NOT_FOUND;
    indexes->second_operand_index = NOT_FOUND;
    indexes->data_index = NOT_FOUND;
}
static void initialize_line(line* sentence){
    sentence->flags.is_empty_line = FALSE;
    sentence->flags.is_data = FALSE;
    sentence->flags.is_code = FALSE;
    sentence->flags.is_comment = FALSE;
    sentence->code_parts.opcode = NOT_FOUND;
    sentence->code_parts.function = NOT_FOUND;
    *(sentence->data_parts.order) = '\0';
    sentence->label.address = 0;
    *(sentence->label.name) = '\0';
}
