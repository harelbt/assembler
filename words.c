#include "words.h"
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
}