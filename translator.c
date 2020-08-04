#include <stdio.h>
#include "translator.h"
void first_pass_translation(FILE* machine_code, line* sentence, line_indexes* indexes, line_counters* counters){
    if (sentence->flags.is_code == TRUE){
        code_translation(machine_code, sentence, indexes, counters);
    } else{
        data_translation(sentence, indexes, counters);
    }
}
static void code_translation(FILE* machine_code, line* sentence, line_indexes* indexes, line_counters* counters){
    instruction_word instruction_word;
    word second_word;
    word third_word;
    int distance_from_last_IC = counters->IC - counters->last_instruction_address;
    prepare_instruction_word(&instruction_word, sentence, indexes);

}
static void data_translation(line* sentence, line_indexes* indexes, line_counters* counters){

}
static void prepare_instruction_word(instruction_word* to_prepare, line* sentence, line_indexes* indexes){
    to_prepare->ARE = 0b100;
    to_prepare->opcode = sentence->code_parts.opcode;
    to_prepare->function = sentence->code_parts.function;
    to_prepare->source_register = indexes->first_operand_index == indexes->first_register_index ?
                                 *(sentence->line + indexes->first_register_index + 1) : 0b0;
    to_prepare->dest_register = indexes->second_operand_index == indexes->second_register_index ?
                               *(sentence->line + indexes->second_register_index + 1) : 0b0;
}
static void prepare_extra_words(const char* line, line_indexes indexes, int num_of_words, ...){
    va_list arg_pointer;
    va_start(arg_pointer, num_of_words);
    if (num_of_words == 2){
        word second_word = va_arg(arg_pointer, word);

    } else{
        word second_word = va_arg(arg_pointer, word);
        word third_word = va_arg(arg_pointer, word);
    }
    va_end(arg_pointer);
}
static void code_word(word word, const char* line, int index){
    switch (*(line + index)) {
        case '#':{}
        case '&':{}
        default:{}
    }
}
static void code_number(word* word, const char* line, int index){
    index++;
    char* to_code;
    switch (*(line + index)) {
        case '-':{
            index++;
            to_code = get_until_white_char(line, index);
            word->number.number = strtod(to_code, NULL);
            word->number.ARE = ABSOLUTE;
        }
        case '+':{
        }
        default:{}
    }
}