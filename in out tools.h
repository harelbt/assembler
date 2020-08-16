#ifndef ASSEMBLER_IN_OUT_TOOLS_H
#define ASSEMBLER_IN_OUT_TOOLS_H
#include <stdio.h>
#include "assembler data types.h"
/*program stop kinds*/
#define MEMORY 12
#define FOPEN 14

/*pointer NULL check*/
#define POINTER_CHECK if (p == NULL) stop(MEMORY,"");

/*pointer types*/
#define CHAR 1
#define SYMBOL 2
#define DATA_IMAGE 3

/*lengths*/
#define LINE_ASSUMED_LENGTH 100
#define TYPE_MAX_LENGTH 4
#define HEX_PRINT_LENGTH 6

/*prints*/
#define PRINT_ADDRESS fprintf(machine_code, "%07d ", last_IC)
#define PRINT_LABEL fprintf(machine_code, "?%s", label_name)
#define PRINT_CODE_WORD fprintf(machine_code, "%06x\n", to_print)
#define PRINT_DATA_WORD fprintf(machine_code,"%07d %06x\n", counters->last_instruction_address + data->DC, data_to_print)
#define PRINT_LAST_DATA_WORD fprintf(machine_code,"%07d %06x", counters->last_instruction_address + data->DC, data_to_print)

/*general*/
#define START 0

/*FUNCTIONS DECLARATION*/
/**
 * gets a line from a file using dynamic memory allocation. returns a pointer to a string (char array)
 * @param file FILE pointer to the file to get the line from
 * @return the line in a dynamic allocated string
 */
char* get_line_dynamic(FILE* file, int* length);
int read_line(FILE* file, line* sentence);
/**
 * like malloc, but in case of a failure, gives a proper massage and exit the program, and does the casting
 * and all the hard work, and works with all of the data types
 * @param size to allocate
 * @param arr_type
 * @return a pointer to the new array
 */
void* allocate_memory (int size, char arr_type);
/**
 * like realloc, but in case of a failure, gives a proper massage and exit the program, and does the casting
 * and all the hard work, and works with all of the data types
 * @param ptr pointer to the array to reallocate
 * @param size to allocate
 * @param arr_type
 * @return a pointer to reallocated new array
 */
void* realloc_memory (void* ptr, int size, char arr_type);
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
void print_visual_indication(int index, const char* line);
int find_next_word(const char* line, int index);
void check_number_appearance(short int* did_number_appeared, char curr_char);
/**
 * returns a string until a white character.
 * if starts in a white character, returns empty string
 * @param file to read from
 * @return a string until a white space
 */
char* get_until_white_char(const char* line, int index);
FILE * open_machine_code(char* file_name, const char* mode);
char* get_file_name_without_type(char* file_name);
void remove_output_files(char* file_name);
void remove_ob_file(char* file_name);
void remove_ent_file(char* file_name);
void remove_ext_file(char* file_name);
FILE* create_ent_files(char* name_without_type);
FILE* create_ext_files(char* name_without_type);
void print_entry_extern(FILE* file, symbol* entry_extern);
void remove_unnecessary_files(char* name_without_type, const char* error_found, const char* is_external, const char* is_entry);
/*printing functions*/
void print_code_words(FILE* machine_code, char* line, line_indexes* indexes, int last_IC, int num_of_words, ...);
void print_label(FILE* machine_code, const char* line, word* word_to_print);
void print_data(FILE* machine_code,data_image* data, line_counters* counters);
void print_words_count(FILE* machine_code, line_counters* counters);
void unite_temp_with_machine_code(FILE* temp_machine_code, FILE* machine_code);
#endif //ASSEMBLER_IN_OUT_TOOLS_H
