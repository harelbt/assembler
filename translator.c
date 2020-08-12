#include <stdio.h>
#include "translator.h"
#include "data image.h"
#include "in out tools.h"
void first_pass_translation(FILE* machine_code, line* sentence, line_indexes* indexes, line_counters* counters, data_image* data){
    if (sentence->flags.is_code == TRUE){
        code_translation(machine_code, sentence, indexes, counters);
    } else if (*(sentence->data_parts.order) == 's' || *(sentence->data_parts.order) == 'd'){
        data_translation(sentence, indexes, counters, data);
    }
}
static void code_translation(FILE* machine_code, line* sentence, line_indexes* indexes, line_counters* counters){
    word instruction_word;
    word second_word;
    word third_word;
    int distance_from_last_IC = counters->IC - counters->last_instruction_address;
    prepare_instruction_word(&instruction_word, sentence, indexes);
    if (distance_from_last_IC == 2) {
        prepare_extra_words(sentence->line, indexes, distance_from_last_IC, &second_word);
        print_code_words(machine_code, sentence->line, indexes, counters->last_instruction_address, distance_from_last_IC, &instruction_word, &second_word);
    } else{
        prepare_extra_words(sentence->line, indexes, distance_from_last_IC, &second_word, &third_word);
        print_code_words(machine_code, sentence->line, indexes, counters->last_instruction_address, distance_from_last_IC, &instruction_word, &second_word, &third_word);
    }
}
static void data_translation(line* sentence, line_indexes* indexes, line_counters* counters, data_image* data){
    if (indexes->first_quotation_mark_index != NOT_FOUND){
        translate_string_sequence(sentence->line + indexes->data_index, counters->last_data_address,indexes->second_quotation_mark_index-indexes->data_index, data);
    } else{
        translate_numbers_sequence(sentence->line + indexes->data_index, indexes, counters->last_data_address, data);
    }
}
static void translate_numbers_sequence(const char* numbers_sequence, line_indexes* indexes, int last_DC, data_image* data){
    int index = 0;
    int DC_addition = 0;
    while (*(numbers_sequence + index)){
        data_image* new_data = allocate_arr_memory(1, DATA_IMAGE);
        new_data->DC = last_DC + DC_addition;
        new_data->next = NULL;
        code_word(&new_data->word, numbers_sequence, index, indexes, DATA);
        insert_data_node(data, new_data);
        get_to_next_number(&index, numbers_sequence);
        DC_addition++;
    }
}
static void translate_string_sequence(const char* string_sequence, int last_DC, int second_quotemark_index, data_image* data){
    int index = 1;
    while (index != second_quotemark_index){
        data_image* new_data = allocate_arr_memory(1, DATA_IMAGE);
        new_data->DC = last_DC + index;
        new_data->next = NULL;
        new_data->word.word = *(string_sequence + index);
        insert_data_node(data, new_data);
        index++;
    }
}
static void prepare_instruction_word(word* to_prepare, line* sentence, line_indexes* indexes){
    to_prepare->word = 0;
    to_prepare->word |= ABSOLUTE;
    set_opcode(to_prepare, sentence->code_parts.opcode);
    set_function(to_prepare, sentence->code_parts.function);
    if (indexes->first_operand_index == NOT_FOUND && indexes->second_operand_index != NOT_FOUND){
        prepare_dest_only_instruction(to_prepare, sentence->line, indexes);
    } else {
        prepare_full_instruction_word(to_prepare, indexes, sentence->line);
    }
}
static void prepare_full_instruction_word(word* to_prepare, line_indexes* indexes, const char* line){
    if (indexes->first_operand_index == indexes->first_register_index) {
        set_source_register(to_prepare, line, indexes->first_register_index);
    }
    if (indexes->second_operand_index == indexes->first_register_index){
        set_dest_register(to_prepare, line, indexes->first_register_index);
    }
    if (indexes->second_operand_index == indexes->second_register_index) {
        set_dest_register(to_prepare, line, indexes->second_register_index);
    }
    set_source_addressing(line, to_prepare, indexes);
    set_dest_addressing(to_prepare, line, indexes);

}
static void prepare_dest_only_instruction(word* to_prepare, const char* line, line_indexes* indexes){
    if (indexes->second_operand_index == indexes->first_register_index) {
        set_dest_register(to_prepare, line, indexes->first_register_index);
    }
    set_dest_addressing(to_prepare, line, indexes);
}
static void set_function(word* word, int function){
    switch (function) {
        case 1:{
            word->word |= FUNCTION_ONE;
            break;
        }
        case 2:{
            word->word |= FUNCTION_TWO;
            break;
        }
        case 3:{
            word->word |= FUNCTION_THREE;
            break;
        }
        case 4:{
            word->word |= FUNCTION_FOUR;
            break;
        }
    }
}
static void set_opcode(word* word, int opcode){
    switch (opcode) {
        case 1:{
            word->word |= OPCODE_ONE;
            break;
        }
        case 2:{
            word->word |= OPCODE_TWO;
            break;
        }
        case 4:{
            word->word |= OPCODE_FOUR;
            break;
        }
        case 5:{
            word->word |= OPCODE_FIVE;
            break;
        }
        case 9:{
            word->word |= OPCODE_NINE;
            break;
        }
        case 12:{
            word->word |= OPCODE_TWELVE;
            break;
        }
        case 13:{
            word->word |= OPCODE_THIRTEEN;
            break;
        }
        case 14:{
            word->word |= OPCODE_FOURTEEN;
            break;
        }
        case 15:{
            word->word |= OPCODE_FIFTEEN;
            break;
        }
    }
}
static void set_source_addressing(const char* line, word* word, line_indexes* indexes){
    int addressing = get_source_addressing(line, indexes);
    switch (addressing) {
        case 1:{
            word->word |= FIRST_SOURCE_ADDRESSING;
            break;
        }
        case 2:{
            word->word |= SECOND_SOURCE_ADDRESSING;
            break;
        }
        case 3:{
            word->word |= THIRD_SOURCE_ADDRESSING;
            break;
        }
    }
}
static void set_dest_addressing(word* word, const char* line, line_indexes* indexes){
    int addressing = get_dest_addressing(line, indexes);
    switch (addressing) {
        case 1:{
            word->word |= FIRST_DEST_ADDRESSING;
            break;
        }
        case 2:{
            word->word |= SECOND_DEST_ADDRESSING;
            break;
        }
        case 3:{
            word->word |= THIRD_DEST_ADDRESSING;
            break;
        }
    }
}
static void set_source_register(word* word, const char* line, int register_index){
    switch (*(line + register_index + 1)) {
        case '1':{
            word->word |= SOURCE_REGISTER_ONE;
            break;
        }
        case '2':{
            word->word |= SOURCE_REGISTER_TWO;
            break;
        }
        case '3':{
            word->word |= SOURCE_REGISTER_THREE;
            break;
        }
        case '4':{
            word->word |= SOURCE_REGISTER_FOUR;
            break;
        }
        case '5':{
            word->word |= SOURCE_REGISTER_FIVE;
            break;
        }
        case '6':{
            word->word |= SOURCE_REGISTER_SIX;
            break;
        }
        case '7':{
            word->word |= SOURCE_REGISTER_SEVEN;
            break;
        }
    }
}
static void set_dest_register(word* word, const char* line, int register_index){
    switch (*(line + register_index + 1)) {
        case '1':{
            word->word |= DEST_REGISTER_ONE;
            break;
        }
        case '2':{
            word->word |= DEST_REGISTER_TWO;
            break;
        }
        case '3':{
            word->word |= DEST_REGISTER_THREE;
            break;
        }
        case '4':{
            word->word |= DEST_REGISTER_FOUR;
            break;
        }
        case '5':{
            word->word |= DEST_REGISTER_FIVE;
            break;
        }
        case '6':{
            word->word |= DEST_REGISTER_SIX;
            break;
        }
        case '7':{
            word->word |= DEST_REGISTER_SEVEN;
            break;
        }
    }
}
static int get_source_addressing(const char* line, line_indexes* indexes){
    if (*(line + indexes->first_operand_index) == '#') {
        return 0;
    } else if (*(line + indexes->first_operand_index) == '&') {
        return 2;
    } else if (indexes->first_operand_index == indexes->first_register_index){
        return 3;
    } else{
        return 1;
    }
}
static int get_dest_addressing(const char* line, line_indexes* indexes){
    if (*(line + indexes->second_operand_index) == '#') {
        return 0;
    } else if (*(line + indexes->second_operand_index) == '&') {
        return 2;
    } else if (indexes->second_operand_index == indexes->first_register_index ||
    indexes->second_operand_index == indexes->second_register_index){
        return 3;
    } else{
        return 1;
    }
}
static void prepare_extra_words(const char* line, line_indexes* indexes, int num_of_words, ...){
    va_list arg_pointer;
    va_start(arg_pointer, num_of_words);
    if (num_of_words == 2){
        int index;
        word* second_word = va_arg(arg_pointer, word*);
        if(indexes->second_operand_index != NOT_FOUND && indexes->second_operand_index == indexes->first_register_index){
            index = indexes->first_operand_index;
        } else{
            index = indexes->second_operand_index;
        }
        code_word(second_word, line, index, indexes, CODE);
    } else{
        word* second_word = va_arg(arg_pointer, word*);
        word* third_word = va_arg(arg_pointer, word*);
        code_word(second_word, line, indexes->first_operand_index, indexes, CODE);
        code_word(third_word, line, indexes->second_operand_index, indexes, CODE);
    }
    va_end(arg_pointer);
}
static void code_word( word* word, const char* line, int index, line_indexes* indexes, char mode){
    if (mode == CODE){
        code_instruction_word(word, line, index, indexes);
    } else{
        code_number(word, line, index, DATA);
    }
}
static void code_instruction_word(word* word, const char* line, int index, line_indexes* indexes){
    word->label_index = NOT_FOUND;
    word->is_label = FALSE;
    word->is_jump = FALSE;
    switch (*(line + index)) {
        case '#':{
            code_number(word, line, index, CODE);
            break;
        }
        case '&':{
            word->is_jump = TRUE;
            word->label_index = index;
            break;
        }
        default:{
            if (index != indexes->first_register_index && index != indexes->second_register_index) {
                word->is_label = TRUE;
                word->label_index = index;
            }
        }
    }
}
static void code_number(word* word, const char* line, int index, char mode){
    char is_sign = FALSE;
    if (mode == CODE) {
        index++;
    }
    char* to_code;
    if (*(line + index) == '-' || *(line + index) == '+'){
        is_sign = TRUE;
        index++;
    }
    to_code = get_number(line, index);
    word->word = strtod(to_code, NULL);
    if (is_sign == TRUE){
        word->word ^= (unsigned int) TWOS_COMP_MASK;
    }
    if (mode == CODE) {
        word->word <<= 3u;
        word->word |= ABSOLUTE;
    }
}
static void print_code_words(FILE* machine_code, char* line, line_indexes* indexes, int last_IC, int num_of_words, ...) {
    int i = num_of_words;
    va_list arg_pointer;
    word *word_to_print;
    va_start(arg_pointer, num_of_words);
    while (i) {
        PRINT_ADDRESS
        word_to_print = va_arg(arg_pointer, word*);
        printf("%i ", last_IC);
        if (word_to_print->is_label == TRUE || word_to_print->is_jump == TRUE) {
            print_label(machine_code, line, indexes, word_to_print);
        } else {/*prints prepared word*/
            non_label_print(word_to_print, machine_code);
        }
        i--;
        last_IC++;
    }
}
static void non_label_print(word* word_to_print, FILE* machine_code){
    unsigned int to_print;
    unsigned int translation_mask = FOUR_BINARIES_TO_HEX;
    char i = 0;
    unsigned int k = 20;
    to_print = word_to_print->word;
    while (i < 6) {
        fprintf(machine_code, "%x", (to_print & translation_mask) >> k);
        printf("%x", (to_print & translation_mask) >> k);
        translation_mask = (translation_mask >> 4u);
        i++;
        k -= 4;
    }
    fprintf(machine_code, "\n");
    printf("\n");
}
static void print_label(FILE* machine_code, const char* line, line_indexes* indexes, word* word_to_print){
    char *label_name = get_until_white_char(line, word_to_print->label_index);
        fprintf(machine_code, "?%s\n", label_name);
        printf("?%s\n", label_name);
}
static char* get_number(const char* line, int index){
    char* number_str = allocate_arr_memory(1, CHAR);
    int i = index;
    int k = 0;
    char curr_char = *(line + i);
    while (curr_char && curr_char != ' ' && curr_char != '\t' && curr_char != ','){
        *(number_str + k) = curr_char;
        k++;
        i++;
        curr_char = *(line + i);
        number_str = realloc_arr_memory(number_str, k+1, CHAR);
    }
    *(number_str + k) = '\0';
    return number_str;
}
static void get_to_next_number(int* index, const char* line){
    char curr_char = *(line + *index);
    while (curr_char && curr_char != ' ' && curr_char != '\t' && curr_char != ','){
        (*index)++;
        curr_char = *(line + *index);
    }
    while (curr_char && curr_char == ' ' || curr_char == '\t' || curr_char == ','){
        (*index)++;
        curr_char = *(line + *index);
    }
}