#ifndef WORDS_WORDS_H
#define WORDS_WORDS_H
#include "helpfunctions.h"
#define WORD_MAX_VAL 8388607
#define WORD_MIN_VAL -8388607
typedef struct {
    char binary[25];
}word;
/**
 * turns a word into hex
 * @param to_convert the word to convert
 * @return a string of the hex represantation of the word
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