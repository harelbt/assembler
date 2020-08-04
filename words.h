#ifndef WORDS_WORDS_H
#define WORDS_WORDS_H
#include "helpfunctions.h"
#define ONE_WORD 1
#define ABSOLUTE
#define TWOS_COMP_MASK 0b1111111111111111111110
typedef struct __attribute__((packed)){
    unsigned int ARE: 3;
    unsigned int function: 5;
    unsigned int dest_register: 3;
    unsigned int dest_addressing: 2;
    unsigned int source_register: 3;
    unsigned int source_addressing: 2;
    unsigned int opcode: 6;
}instruction_word;
typedef union __attribute__((packed)){
     struct {
        unsigned int number : 21;
         unsigned int ARE : 3;
     }number;
    struct {
        unsigned int address : 21;
        unsigned int ARE : 3;
    }address;
    struct {
        unsigned int distance : 21;
        unsigned int ARE : 3;
    }distance;
}word;
void calculate_number_of_words(line* sentence, line_indexes indexes, line_counters* counters);
void calculate_instruction_word(line* sentence, line_indexes indexes, line_counters* counters);
void calculate_order_word(line* sentence, line_indexes indexes, line_counters* counters);
/**
 * turns a word into hex
 * @param to_convert the word to convert
 * @return a string of the hex representation of the word
 */
char* word_to_hex(instruction_word to_convert);
/**
 * takes a decimal value and makes it a word
 * @param to_convert the number to convert
 * @return the word
 */
instruction_word number_to_word(long to_convert);
/**
 * fills a word with a given string and fills the rest with 0 or 1, depends on the negative parameter
 * @param str to fill the word with
 * @param to_fill the word to fill
 * @param negative tells if the value from the string is negative
 */
void fill_word(const char *str, instruction_word *to_fill, short int negative);
#endif