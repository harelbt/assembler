#ifndef WORDS_WORDS_H
#define WORDS_WORDS_H
#include "helpfunctions.h"
#define INT_MAX_VAL 8388607
#define INT_MIN_VAL -8388607
#define ONE_WORD 1
typedef struct __attribute__((packed)){
    unsigned int ARE: 3;
    unsigned int FUNCTION: 5;
    unsigned int DEST_REGISTER: 3;
    unsigned int DEST_ADDRESSING: 2;
    unsigned int SOURCE_REGISTER: 3;
    unsigned int SOURCE_ADDRESSING: 2;
    unsigned int OPCODE: 6;
}word;
void calculate_number_of_words(line* sentence, line_marks_index indexes, line_marks_counter* counters);
void calculate_instruction_word(line* sentence, line_marks_index indexes, line_marks_counter* counters);
void calculate_order_word(line* sentence, line_marks_index indexes, line_marks_counter* counters);
/**
 * turns a word into hex
 * @param to_convert the word to convert
 * @return a string of the hex representation of the word
 */
char* word_to_hex(word to_convert);
/**
 * takes a decimal value and makes it a word
 * @param to_convert the number to convert
 * @return the word
 */
word number_to_word(long to_convert);
/**
 * fills a word with a given string and fills the rest with 0 or 1, depends on the negative parameter
 * @param str to fill the word with
 * @param to_fill the word to fill
 * @param negative tells if the value from the string is negative
 */
void fill_word(const char *str, word *to_fill, short int negative);
#endif