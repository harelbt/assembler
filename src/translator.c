#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "translator.h"
#include "in_out_tools.h"
#include "symbol table.h"
#include "errors.h"
/*
 * translation for the first pass. translates the source file to a semi ready machine code.
 * labels are printed in ?LABEL format(if the label's name is LABEL).
 * requires the data image
 */
void first_pass_translation(FILE* machine_code, line* sentence, line_indexes* indexes, line_counters* counters, data_image* data){
    if (sentence->flags.is_code == TRUE){
        code_translation(machine_code, sentence, indexes, counters);
    } else if (*(sentence->data_parts.order) == 's' || *(sentence->data_parts.order) == 'd'){
        data_translation(sentence, indexes, counters, data);
    }
}
/*instruction translation functions*/
/*
 * FIRST PASS LEVEL
 *translates a code(instruction) line to the machine code file.
 */
void code_translation(FILE* machine_code, line* sentence, line_indexes* indexes, line_counters* counters){
    /*struct word contains a 24 bits bitfield and required flag for translating words*/
    word instruction_word;/*to code the instruction line itself*/
    /*instruction line can have max of three words*/
    word second_word;
    word third_word;
    /*distance from last ic is used to know how many words are needed*/
    int distance_from_last_IC = counters->IC - counters->last_IC;
    /*coding instruction word*/
    prepare_instruction_word(&instruction_word, sentence, indexes);
    if(distance_from_last_IC == 1){/*only instruction word is required*/
        print_code_words(machine_code, sentence->line, counters->last_IC, distance_from_last_IC, &instruction_word);
        return;
    }
    if (distance_from_last_IC == 2) {/*2 words are required*/
        prepare_extra_words(sentence->line, indexes, counters, distance_from_last_IC, &second_word);
        print_code_words(machine_code, sentence->line, counters->last_IC, distance_from_last_IC, &instruction_word, &second_word);
    } else{/*3 words are required*/
        prepare_extra_words(sentence->line, indexes, counters, distance_from_last_IC, &second_word, &third_word);
        print_code_words(machine_code, sentence->line, counters->last_IC, distance_from_last_IC, &instruction_word, &second_word, &third_word);
    }
}
/*
 * FIRST PASS LEVEL
 * codes instruction line to a word in a struct word
 */
void prepare_instruction_word(word* to_prepare, line* sentence, line_indexes* indexes){
    to_prepare->word = 0;/*sets bitfield to zeros*/
    to_prepare->word |= ABSOLUTE;/*turns A bit on*/
    set_opcode(to_prepare, sentence->code_parts.opcode);
    set_function(to_prepare, sentence->code_parts.function);
    /*if there's only one operand*/
    if (indexes->first_operand_index == NOT_FOUND && indexes->second_operand_index != NOT_FOUND){
        prepare_dest_only_instruction(to_prepare, sentence->line, indexes);
    } else {
        prepare_full_instruction_word(to_prepare, indexes, sentence->line);
    }
    to_prepare->is_jump = FALSE;
    to_prepare->is_label = FALSE;
}
/*
 * FIRST PASS LEVEL
 * codes a line with only a destination operand
 */
void prepare_dest_only_instruction(word* to_prepare, const char* line, line_indexes* indexes){
    /*if the only operand is a register sets the register area in the bitfield.
     * other operands leaves this area as zeros*/
    if (indexes->second_operand_index == indexes->first_register_index) {
        set_dest_register(to_prepare, line, indexes->first_register_index);
    }
    set_dest_addressing(to_prepare, line, indexes);
}
/*
 * FIRST PASS LEVEL
 * codes a line with 2 or 0 operands
 */
void prepare_full_instruction_word(word* to_prepare, line_indexes* indexes, const char* line){
    /*if the first operand is a register, sets the source register area in the bitfield*/
    if (indexes->first_operand_index == indexes->first_register_index) {
        set_source_register(to_prepare, line, indexes->first_register_index);
    }
    /*if the second operand is the first register, sets the dest register area in the bitfield
    *according to the first register */
    if (indexes->second_operand_index == indexes->first_register_index){
        set_dest_register(to_prepare, line, indexes->first_register_index);
    }
    /*if the second operand is the second register, sets the dest register area in the bitfield
    *according to the second register */
    if (indexes->second_operand_index == indexes->second_register_index) {
        set_dest_register(to_prepare, line, indexes->second_register_index);
    }
    set_source_addressing(line, to_prepare, indexes);
    set_dest_addressing(to_prepare, line, indexes);
}
/*
 * FIRST PASS LEVEL
 * codes the extra word of an instruction word.
 * struct word to code to can be given as ... arguments(in the end of the signature)
 * requires a number of word for the line (1-3)
 */
void prepare_extra_words(const char* line, line_indexes* indexes, line_counters* counters, int num_of_words, ...){
    va_list arg_pointer;/*"..." argument pointer*/
    va_start(arg_pointer, num_of_words);
    if (num_of_words == 2){/*num of extra words to code is ONE*/
        int index;/*to store */
        word* second_word = va_arg(arg_pointer, word*);
        /*if the there are two operands in the line, and the second one is a register(not requires a word),
         * then the operand that needs the word is the first one. else, it's the second one*/
        if(indexes->second_operand_index != NOT_FOUND && indexes->second_operand_index == indexes->first_register_index){
            index = indexes->first_operand_index;
        } else{
            index = indexes->second_operand_index;
        }
        code_word(second_word, line, index, indexes, counters, CODE);/*codes the word according to the given index*/
    } else{/*num of word = 3, codes the 2 extra words*/
        word* second_word = va_arg(arg_pointer, word*);
        word* third_word = va_arg(arg_pointer, word*);
        code_word(second_word, line, indexes->first_operand_index, indexes, counters, CODE);
        code_word(third_word, line, indexes->second_operand_index, indexes, counters, CODE);
    }
    va_end(arg_pointer);
}
/*
 * FIRST PASS LEVEL
 * codes one operand or one number to one struct word
 * requires a "mode" flag (CODE or DATA macros) to know the type of the coding
 */
void code_word( word* word, const char* line, int index, line_indexes* indexes,line_counters* counters, char mode){
    if (mode == CODE){
        code_instruction_word(word, line, index, indexes, counters);
    } else{
        code_number(word, line, index, DATA);
    }
}
/*
 * FIRST PASS LEVEL
 * codes one operand to one struct word
 */
void code_instruction_word(word* word, const char* line, int index, line_indexes* indexes, line_counters* counters){
    /*resets the label flags for later use*/
    word->label_index = NOT_FOUND;
    word->is_label = FALSE;
    word->is_jump = FALSE;
    /*checks the operand's type, and acts accordingly*/
    switch (*(line + index)) {
        case '#':{
            code_number(word, line, index, CODE);
            break;
        }
        case '&':{
            word->is_jump = TRUE;
            word->label_index = index;
            insert_symbol_usage_line(counters);/*saves the line number for the second pass*/
            break;
        }
        default:{/*if it's a regular label(not '&', '#' or register(the condition below))*/
            if (index != indexes->first_register_index && index != indexes->second_register_index) {
                word->is_label = TRUE;
                word->label_index = index;
                insert_symbol_usage_line(counters);/*saves the line number for the second pass*/
            }
        }
    }
}
/*data translation functions*/
/* FIRST PASS LEVEL
 *translates an order line to the machine code file.
 */
void data_translation(line* sentence, line_indexes* indexes, line_counters* counters, data_image* data){
    if (indexes->first_quotation_mark_index != NOT_FOUND){/*if a string exists, translates a string*/
        translate_string_sequence(sentence->line + indexes->data_index, counters->last_DC, indexes->second_quotation_mark_index - indexes->data_index, data);
    } else{/*translates data*/
        translate_numbers_sequence(sentence->line + indexes->data_index, indexes, counters, counters->last_DC, data);
    }
}
/*
 * FIRST PASS LEVEL
 * translates a sequence of numbers(after a .data order)
 * requires the number sequence as a string
 */
void translate_numbers_sequence(const char* numbers_sequence, line_indexes* indexes, line_counters* counters, int last_DC, data_image* data){
    int index = 0;
    int DC_addition = 1;/*indicates the count of number to code and whats it's address relatively to the last dc*/
    while (*(numbers_sequence + index)){/*while the string didn't end*/
        /*creates data image node to save the data*/
        data_image* new_data = allocate_memory(ONE_UNIT, DATA_IMAGE);
        /*sets the DC(last dc + count of number to code)*/
        new_data->DC = last_DC + DC_addition;
        new_data->next = NULL;
        code_word(&new_data->word, numbers_sequence, index, indexes, counters, DATA);
        insert_data_node(data, new_data);
        get_to_next_number(&index, numbers_sequence);/*for the next loop*/
        DC_addition++;
    }
}
/*
 * FIRST PASS LEVEL
 * translates a sequence of chars(after a .string order)
 * requires the chars sequence as a string
 */
void translate_string_sequence(const char* string_sequence, int last_DC, int second_quotemark_index, data_image* data){
    int index = 1;/*counts the chars to code*/
    data_image* last_data_node;
    while (index != second_quotemark_index){/*while the string hasn't ended*/
        data_image* new_data = allocate_memory(ONE_UNIT, DATA_IMAGE);
        translate_character(new_data, *(string_sequence + index), last_DC, index);
        insert_data_node(data, new_data);
        index++;
    }
    /*inserts a '\0'*/
    last_data_node = allocate_memory(ONE_UNIT, DATA_IMAGE);
    translate_character(last_data_node, 0, last_DC, index);
    insert_data_node(data, last_data_node);
}
/*
 * FIRST PASS LEVEL
 * translates a character to a word
 */
void translate_character(data_image* new_data, char character, int last_DC, int index) {
    new_data->DC = last_DC + index;/*DC is the last DC + the char count*/
    new_data->next = NULL;
    new_data->word.word = character;/*code is the ascii*/
}
/*
 * FIRST PASS LEVEL
 * returns the string of the number in a string that starts in a given index
 */
char* get_number(const char* line, int index){
    char* number_str = allocate_memory(NUMBER_ALLOWED_LENGTH, CHAR);/*to return*/
    int i = index;
    int k = 0;/*to fill the number string*/
    char curr_char = *(line + i);
    while (curr_char && curr_char != ' ' && curr_char != '\t' && curr_char != ','){/*until the nuber ends*/
        *(number_str + k) = curr_char;/*fills number string*/
        k++;
        i++;
        curr_char = *(line + i);
    }
    *(number_str + k) = '\0';
    return number_str;
}
/*
 * FIRST PASS LEVEL
 * gets to the next number in a string and stores the index in the "index" argument
 */
void get_to_next_number(int* index, const char* line){
    char curr_char = *(line + *index);
    /*skips current number*/
    while (curr_char && curr_char != ' ' && curr_char != '\t' && curr_char != ','){
        (*index)++;
        curr_char = *(line + *index);
    }
    /*skips white chars and commas, to the next number*/
    while (curr_char && (curr_char == ' ' || curr_char == '\t' || curr_char == ',')){
        (*index)++;
        curr_char = *(line + *index);
    }
}
/*
 * FIRST PASS LEVEL
 * codes one number to a struct word. can code a number with/out # and with signs
 * requires a "mode" flag (CODE/DATA) to know how to work with # and ARE
 */
void code_number(word* word, const char* line, int index, char mode){
    char is_minus_sign = FALSE;/*indicates minus sign existance. updates next*/
    char* to_code;/*will store the number string*/
    /*number in an instruction line has #, skips it(index++)*/
    if (mode == CODE) {
        index++;

    }
    /*skips the sign if exists*/
    if (*(line + index) == '-' || *(line + index) == '+'){
        if (*(line + index) == '-') {
            is_minus_sign = TRUE;
        }
        index++;
    }
    to_code = get_number(line, index);
    word->word = strtod(to_code, NULL);/*codes number to the struct word*/
    /*two's complement representation for negative numbers*/
    if (is_minus_sign == TRUE){
        word->word ^= ONES_COMP_MASK;
        word->word += COMPLEMENT_TO_TWO;
    }
    /*adds A bit to a number in an instruction line*/
    if (mode == CODE) {
        word->word <<= 3u;
        word->word |= ABSOLUTE;
    }
    free(to_code);
    to_code = NULL;
}
/*
 * inserts data image node to the data image
 */
void insert_data_node(data_image* head, data_image* to_insert){
    data_image* curr_pointer = head;
    /*if it's the first data node entered, fills the head instead of linking it to the head*/
    if (head->is_head_filled == FALSE){
        head->word = to_insert->word;
        head->DC = to_insert->DC;
        head->is_head_filled = TRUE;
        free(to_insert);
        to_insert = NULL;
        return;
    }
    while (curr_pointer->next != NULL){/*skips to the end*/
        curr_pointer = curr_pointer->next;
    }
    curr_pointer->next = to_insert;/*inserts*/
}
/*set instruction bitfield functions*/
/*
 * FIRST PASS LEVEL
 * sets the opcode area in a given struct word's bitfield, according to a given opcode
 */
void set_opcode(word* word, int opcode){
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
/*
 * FIRST PASS LEVEL
 * sets the function area in a given struct word's bitfield, according to a given function
 */
void set_function(word* word, int function){
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
/*
 * FIRST PASS LEVEL
 * sets the destination addressing in a given struct word's bitfield.
 * requires the code line
 */
void set_dest_addressing(word* word, const char* line, line_indexes* indexes){
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
/*
 * FIRST PASS LEVEL
 * sets the source addressing in a given struct word's bitfield.
 * requires the code line
 */
void set_source_addressing(const char* line, word* word, line_indexes* indexes){
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
/*
 * FIRST PASS LEVEL
 * sets the source register in a given struct word's bitfield.
 * requires the code line and the register index
 */
void set_source_register(word* word, const char* line, int register_index){
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
/*
 * FIRST PASS LEVEL
 * sets the destination register in a given struct word's bitfield.
 * requires the code line and the register index
 */
void set_dest_register(word* word, const char* line, int register_index){
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
/*addressing discover functions*/
/*
 * FIRST PASS LEVEL
 * returns the source addressing code according to the code line and indexes
 */
int get_source_addressing(const char* line, line_indexes* indexes){
    /*no operand or a number*/
    if (indexes->first_operand_index == NOT_FOUND || *(line + indexes->first_operand_index) == '#') {
        return 0;
    } else if (*(line + indexes->first_operand_index) == '&') {/*"jump to label"*/
        return 2;
    } else if (indexes->first_operand_index == indexes->first_register_index){/*register*/
        return 3;
    } else{/*regular label*/
        return 1;
    }
}
/*
 * FIRST PASS LEVEL
 * returns the destination addressing code according to the code line and indexes
 */
int get_dest_addressing(const char* line, line_indexes* indexes){
    /*no operand or a number*/
    if ((indexes->first_operand_index == NOT_FOUND && indexes->second_operand_index == NOT_FOUND) || *(line + indexes->second_operand_index) == '#') {
        return 0;
    } else if (*(line + indexes->second_operand_index) == '&') {/*"jump to label"*/
        return 2;
    } else if (indexes->second_operand_index == indexes->first_register_index ||
    indexes->second_operand_index == indexes->second_register_index){/*register*/
        return 3;
    } else{/*regular label*/
        return 1;
    }
}
/*words calculation*/
/*
 * calculates the number of words for the current line.
 * updates the "counters" struct
 */
void calculate_number_of_words(line* sentence, line_indexes indexes, line_counters* counters){
    if (sentence->flags.is_code == TRUE){/*instruction line*/
        calculate_instruction_word(counters);
    }else{/*order line*/
        calculate_order_word(sentence, indexes, counters);
    }
}
/*
 * calculates the number of words for an instruction line
 * updates the "counters" struct
 */
void calculate_instruction_word(line_counters* counters){
    /*every operand costs a word, unless it's a register*/
    counters->last_IC = counters->IC;
    counters->IC += counters->number_of_operands - counters->number_of_registers + ONE_WORD;/*one word for the assembly line*/
}
/*
 * calculates the number of words for an order line
 * updates the "counters" struct
 */
void calculate_order_word(line* sentence, line_indexes indexes, line_counters* counters){
    /*updates only it not .entry/.extern, they are special cases*/
    if (strcmp(sentence->data_parts.order, "extern") != 0 && strcmp(sentence->data_parts.order, "entry") != 0) {
        counters->last_DC = counters->DC;
        /*.data words  = number of commas + 1 (2,3 = 2 word, 5 = 1 word)
         * .string words = second quote mark index - first quote mark index (length + 1(for the '\0'))*/
        counters->DC += counters->number_of_quotation_marks >= 2 ?
                        indexes.second_quotation_mark_index - indexes.first_quotation_mark_index :
                        counters->number_of_commas + 1;
    }
}
/*labels coding*/
/*
 * SECOND PASS LEVEL
 * codes a "jump to label" (& label) to the machine code.
 * requires the string from the start of the label
 * if errors found, turns "error_found" to TRUE, else FALSE
 */
void code_jump(FILE* machine_code, symbol* symbol_table, char* i, line_counters* counters, label_address* label_address, char* curr_address, char* error_found){
    char* symbol_name = get_symbol_name(i);/*without with chars*/
    symbol* symbol_to_code = get_symbol(symbol_table, symbol_name);
    int distance;/*for the distance coding in the machine code*/
    if (symbol_to_code == NULL) {/*if symbols wasn't found, no such label was declared*/
        report_error(i, LABEL_DOESNT_EXIST, counters);
        *error_found = TRUE;
    }
    else {/*if the symbol was found*/
        /*distance calculation*/
        distance = symbol_to_code->address - ((int) strtod(curr_address, NULL) - ONE_WORD);
        if (distance < 0) {/*two's complement representation for a negative distance*/
            label_address->label_address_binary = abs(distance);
            label_address->label_address_binary ^= ONES_COMP_MASK;
            label_address->label_address_binary += COMPLEMENT_TO_TWO;
        }
        label_address->label_address_binary = distance;
        label_address->label_address_binary <<= 3u;
        label_address->label_address_binary |= ABSOLUTE;/*turn on the A bit*/
        /*printing*/
        fprintf(machine_code, "%06x\n",
                (signed int) label_address->label_address_binary);/*write in hexa the labels label_address */
    }
    free(symbol_name);
}
/*
 * SECOND PASS LEVEL
 * codes a label to the machine code.
 * requires the string from the start of the label and is external flag to let the program know
 * if a .ext file needs to be removed
 * if errors found, turns "error_found" to TRUE, else FALSE
 */
void code_label_address(FILE* machine_code, FILE* externals_file, symbol* symbol_table, char* i, line_counters* counters, label_address* label_address, char* curr_address, char* error_found, char* is_external){
    char* symbol_name = get_symbol_name(i);/*without white chars*/
    symbol *symbol_to_code = get_symbol(symbol_table, symbol_name);
    if (symbol_to_code == NULL) {/*if symbols wasn't found, no such label was declared*/
        report_error(i, LABEL_DOESNT_EXIST, counters);
        *error_found = TRUE;
    }
    else {/*if the symbol was found*/
        /*treats external symbol*/
        if (symbol_to_code->external == EXTERN) {
            print_extern(externals_file, symbol_to_code, curr_address);
            *is_external = TRUE;
            label_address->label_address_binary |= EXTERNAL;/*turn on the E bit*/
        } else {/*non-external symbol*/
            label_address->label_address_binary = symbol_to_code->address;/*gets the address to code*/
            if (symbol_to_code->sentence_type == DATA) {/*order line addresses get IC addition*/
                label_address->label_address_binary += counters->IC;
            }
            /*labels are relocatable*/
            label_address->label_address_binary <<= 3U;
            label_address->label_address_binary |= RELOCATABLE;/*turn on the R bit*/
        }
        /*printing*/
        fprintf(machine_code, "%06x\n",
                (signed int) label_address->label_address_binary);/*write in hexa the labels label_address */
    }
    free(symbol_name);
}