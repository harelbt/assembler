#include <stdio.h>
#include "translator.h"
#include "words.h"
void first_pass_translation(line* sentence, line_marks_index* indexes, line_marks_counter* counters){
    if (sentence->flags.is_code == TRUE){
        word first_word;
        first_word.ARE = 0b100;
        first_word.OPCODE = sentence->code_parts.operator_parts.opcode;
        first_word.FUNCTION = sentence->code_parts.operator_parts.function;
        first_word.SOURCE_REGISTER = indexes->first_operand_index == indexes->first_register_index ?
                *(sentence->line + indexes->first_register_index + 1) : 0b0;
        first_word.DEST_REGISTER = indexes->second_operand_index == indexes->second_register_index ?
                                   *(sentence->line + indexes->second_register_index + 1) : 0b0;
    } else{

    }
}
