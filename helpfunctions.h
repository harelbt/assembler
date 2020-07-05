
#ifndef HELP_H
#define HELP_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include "assembler data types.h"
#define INPUT 10
#define OUTPUT 11
#define MEMORY 12
#define OTHER 13
#define FOPEN 14
#define POINTER_CHECK if (p == NULL) stop(MEMORY,"");
#define ON 1
#define OFF 0

/**
 *  returns the first character after the white characters
 *  if starts at a non white character, returns the next character
 *  @param file the file to read from
 * @return the first character after the white characters
 */
int skip_white_chars(FILE* file);
/**
 *  returns the first character after the spaces
 *  if starts at a non space character, returns the next character
 * @param file the file to read from
 * @return the first character after the spaces
 */
int skip_spaces(FILE* file);
/**
 * stops with exit code, optional - custom massage, if a massage isn't provided, ""(empty string) is required instead:
 * 1)with pre defined codes(below) and "" string (pre made massage to stderr).
 * 2)"EXIT_SUCCESS/FAILURE" and "" string(no massage to stderr).
 * "EXIT_SUCCESS/FAILURE" and a massage to stderr.
 * codes were defined as macros:
 * INPUT, OUTPUT, MEMORY, OTHER, FOPEN, EXIT_SUCCESS, EXIT_FAILURE.
 * @param exit_type
 * @param massage
 */
void stop(int exit_type, const char* massage);
/**
 * like fopen, but if the opening failed gives a proper massage and exit the program
 * @param file_name to open
 * @param mode open mode, as in fopen
 * @return a FILE pointer to the file (FILE*)
 */
FILE* open_file(const char* file_name, const char* mode);
/**
 * like malloc, but in case of a failure, gives a proper massage and exit the program, and does the casting
 * and all the hard work, and works with all of the data types
 * @param size to allocate
 * @param arr_type
 * @return a pointer to the new array
 */
void* allocate_arr_memory (int size, const char* arr_type);
/**
 * like realloc, but in case of a failure, gives a proper massage and exit the program, and does the casting
 * and all the hard work, and works with all of the data types
 * @param ptr pointer to the array to reallocate
 * @param size to allocate
 * @param arr_type
 * @return a pointer to reallocated new array
 */
void* realloc_arr_memory (void* ptr, int size, const char* arr_type);
/**
 * returns a string until a white character.
 * if starts in a white character, returns empty string
 * @param file to read from
 * @return a string until a white space
 */
char* get_until_white_char(FILE* file);
/**
 * returns a line and deletes the spaces
 * @param file to read from
 * @return a line without spaces
 */
char* get_line_no_spaces(FILE* file);
/**
 * returns the next word that appears after any number of spaces and enter characters, from it's first word to the last
 * @param file to read from
 * @return the next word
 */
char* get_next_word(FILE* file);
/**
 * translates strings to binary base
 * returns the result in a long array
 * ----
 * every char's translation is stored in a different cell ("12" = |1|10|)
 * ----
 * @param str the string to translate
 * @return long array with a binary translation
 */
long* str_to_binary(const char* str);
/**
 * translates numbers to binary base
 * returns the result in a short int array(example: |1|0|0|1)
 * @param number to translate
 * @return the binary translation in a short int array
 */
short int* dec_to_binary(long number);
/**
 * returns a decimal as a string (char*)
 * @param number to convert
 * @return a string representation of the number (int)
 */
char* decimal_to_str(long number);
/**
 * inserts a digit to a string, in the given index
 * @param str to insert to
 * @param number to insert
 * @param index the index to insert to
 */
void insert_digit_to_str(char* str, int number, long index);
/**
 * takes const char array and returns an "unconsted" COPY
 * @param original_array to unconst
 * @return non const char array
 */
char* strUnconst(const char* original_array);
/**
 * takes array of short ints and returns a string representation
 * @param short_array to convert
 * @param length_or_end_number IF AN END NUMBER IS GIVEN, TYPE "ON"(UPPER) IN end_number_mode PARAMETER
 * @param space_mode puts a space in the string after every cell of the short int array
 * @param end_number_mode tells the function that an end number is given instead of the length
 * @return a string representation of a short int array
 */
char* short_arr_to_str(short int* short_array, long length_or_end_number, int space_mode, int end_number_mode);
/**
 * takes a number and puts every digit in a different cell, in a short int array
 * @param number to store in a short int array
 * @return a short int representation of a number
 */
short * long_to_short_arr(long number);
/**
 * translates binary code (stored in a string(char array)) to text
 * @param to_translate binary code to translate
 * @return translation in a string (char array);
 */
char* binary_to_str(const char* to_translate);
/**
 * tells the length of a short array, until a specific number
 * @param array the array to check it's length
 * @param last_number the number to stop the counting at
 * @return the length of the array until the given number
 */
long short_arr_length(const short int* array, int last_number);
/**
 * gets a line from a file using dynamic memory allocation. thus, returns a pointer to a string (char array)
 * @param file FILE pointer to the file to get the line from
 * @return the line in a dynamic allocated string
 */
char* get_line_dynamic(FILE* file);
/**
 * jumps to the EOF and returns it
 * IN STDIN ONLY THE USER SENDS EOF AND THIS AUTOMATICALLY TERMINATES THE PROGRAM
 * @param file the file to jump to EOF in
 * @return EOF
 */
int get_to_eof(FILE* file);
/**
 * jumps to the end of the line and returns the enter('\n') or the EOF
 * @param file the file to jump to the end of the line in
 * @return the last char encountered (EOF or enter ('\n'))
 */
int get_line(FILE* file);
/**
 * gets max of four digit binary code and returns a char that represent the binary code in char
 * if the number is longer than 5 digits, returns G, which is not part of the hexadecimal base
 * very useful when translating binary to hexadecimal
 * @param binary to translate
 * @return a character representation in hexadecimal base of the given binary code
 */
char four_binaries_to_hex (short int binary);
/**
 * turns a binary value to its negative value using the compliment of 2 method, changes the given string and returns it in edition
 * @param binary to turn negative
 * @param length the length of the short int array that holds the binary code
 * @return the original array with the negative result in a short int array (also changes the original given array)
 */
short int* comp_of_2 (short int* binary,long length);

int find_first_char(line* sentence);
int find_semicolon(line* sentence);
static int read_line(FILE* file, line* sentence);
#endif /*HELP_H*/
