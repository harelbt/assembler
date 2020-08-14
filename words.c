#include <string.h>
#include "words.h"
void calculate_number_of_words(line* sentence, line_indexes indexes, line_counters* counters){
    if (sentence->flags.is_code == TRUE){
        calculate_instruction_word(sentence, indexes, counters);
    }else{
        calculate_order_word(sentence, indexes, counters);
    }
}
void calculate_instruction_word(line* sentence, line_indexes indexes, line_counters* counters){
    /*every operand costs a word, unless it's a register*/
    counters->last_instruction_address = counters->IC;
    counters->IC += counters->number_of_operands - counters->number_of_registers + ONE_WORD;/*one word for the assembly line*/
}
void calculate_order_word(line* sentence, line_indexes indexes, line_counters* counters){
    if (strcmp(sentence->data_parts.order, "extern") != 0 && strcmp(sentence->data_parts.order, "entry") != 0) {
        counters->last_data_address = counters->DC;
        counters->DC += counters->number_of_quotation_marks == 2 ?
                        indexes.second_quotation_mark_index - indexes.first_quotation_mark_index - 1 :
                        counters->number_of_commas + 1;
    }
}
/*
char* word_to_hex(word to_convert){
    char* hex = allocate_arr_memory(7, "char");
    char sub_word[5];
    short int binary;
    short int k = 20;
    char** endp = NULL;
    int i = 5;
    hex[6] = '\0';
    sub_word[4] = '\0';
    while (i >= 0){
        binary = strtod(strncpy(sub_word,(to_convert.binary)+k, 4),endp);
        k -= 4;
        hex[i] = four_binaries_to_hex(binary);
        i--;
    }
    return hex;
}
word number_to_word(long to_convert){
    char* str = short_arr_to_str(dec_to_binary(to_convert),-1,OFF,ON);
    word converted;
    short int negative = 0;
    if (to_convert < 0){
        negative = 1;
    }
    fill_word(str, &converted, negative);
    return converted;
}

void fill_word(const char *str, word *to_fill, short int negative) {
    long i = 23;
    long k = 0;
    long str_length = (long) strlen(str);
    to_fill->binary[24] = '\0';
    while ((str_length -1 -k) >= 0){
        to_fill->binary[i] = str[str_length-1-k];
        i--;
        k++;
    }
    if (negative == 0) {
        while (i >= 0) {
            to_fill->binary[i] = '0';
            i--;
        }
    } else
        while (i >= 0) {
            to_fill->binary[i] = '1';
            i--;
        }
}*/