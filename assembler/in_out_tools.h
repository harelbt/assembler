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
#define PRINT_DATA_WORD fprintf(machine_code,"%07d %06x\n", counters->last_IC + data->DC, data_to_print)
#define PRINT_LAST_DATA_WORD fprintf(machine_code,"%07d %06x", counters->last_IC + data->DC, data_to_print)

/*general*/
#define START 0
/*sizes*/
#define ONE_UNIT 1
/*FUNCTIONS DECLARATION*/
/*strings functions*/
/**
 * gets a line from a file using dynamic memory allocation. returns a pointer to a string (char array).
 * returns an empty string (not dynamically allocated) if the line is EOF
 * @param file FILE pointer to the file to get the line from
 * @return the line in a dynamic allocated string or an empty string (not dynamically allocated) if the line is EOF
 */
char* get_line_dynamic(FILE* file, int* length);
/**
 * reads a line from a file using dynamic memory allocation and stores it in a line*
 * it the line is EOF returns EOF, else returns 0;
 * @param file to read from
 * @param sentence struct line pointer
 * @return it the line is EOF returns EOF, else returns 0;
 */
int read_line(FILE* file, line* sentence);
/**
 * returns the first char after the spaces ' '. return value may be a new line character, consider using the skip_white_chars
 * function.
 * @param file to skip spaces in
 * @return the first char after the spaces.
 */
int skip_spaces(FILE* file);
/**
 * given an index of a white char, returns the index of the first character of the next word
 * @param line char to find the next word in
 * @param index the white char index to find the next word from
 * @return the index of the first character of the next word
 */
int find_next_word(const char* line, int index);
/**
 * returns a string from the file pointer until a white char(dynamic memory allocation)
 * if the file pointer points at a white char, returns "" (empty string)
 * @param file to read from
 * @return a string until a white space
 */
char* get_until_white_char(const char* line, int index);
/*memory allocation functions*/
/**
 * like malloc, but in case of a failure, gives a proper massage  to stderr and exit the program, and does the casting
 * and all the hard work, and works with all of the relevant data types in the program
 * USE MACROS: SYMBOL, CHAR for type argument if the macro is undefined returns NULL
 * @param size to allocate
 * @param type USE MACROS: SYMBOL, CHAR, DATA_IMAGE
 * @return a pointer to the new array
 */
void* allocate_memory (int size, char type);
/**
 * like realloc, but in case of a failure, gives a proper massage to stderr and exit the program, and does the casting
 * and all the hard work, and works with all of the relevant data types in the program
 * USE MACROS: SYMBOL, CHAR for type argument if the macro is undefined returns NULL
 * @param ptr pointer to the array to reallocate
 * @param size to allocate
 * @param type USE MACROS: SYMBOL, CHAR
 * @return a pointer to reallocated new array
 */
void* realloc_memory (void* ptr, int size, char type);
/*files functions*/
/**
 * like fopen, but if the opening failed gives a proper massage to stderr and exit the program
 * @param file_name to open
 * @param mode open mode, as in fopen
 * @return a FILE pointer to the file (FILE*)
 */
FILE* open_file(const char* file_name, const char* mode);
/**
 * opens an .ob file that is known to be the machine code for the imaginary cpu
 * the given name needs to be without a file type (for example .exe), mode argument is like in fopen
 * handles opening errors
 * @param file_name to open
 * @param mode file open mode as in fopen
 * @return FILE* to the file
 */
FILE * open_machine_code(char* file_name, const char* mode);
/**
 * creates .ent file using a file name without file type
 * @param name_without_type the name to open with
 * @return FILE* to the file
 */
FILE* create_ent_files(char* name_without_type);
/**
 * creates .ext file using a file name without file type
 * @param name_without_type the name to open with
 * @return FILE* to the file
 */
FILE* create_ext_files(char* name_without_type);
/**
 * extracts the file name without the file type
 * changes the original string
 * @param file_name the full file name
 * @return the file name without type
 */
char* get_file_name_without_type(char* file_name);
/**
 * removes .ob .ent .ext files using a name without file type
 * @param file_name file name without file type
 */
void remove_output_files(char* file_name);
/**
 * removes "temp.exe" file(used in for making the machine code)
 */
void remove_temp_file();
/**
 * removes .ob file using a name without file type
 * @param file_name file name without file type
 */
void remove_ob_file(char* file_name);
/**
 * removes .ent file using a name without file type
 * @param file_name file name without file type
 */
void remove_ent_file(char* file_name);
/**
 * removes .ext file using a name without file type
 * @param file_name file name without file type
 */
void remove_ext_file(char* file_name);
/**
 * checks which files are unnecessary and removes them(.ob, .ext, .ent)
 * @param name_without_type file name without file type
 * @param error_found needs to store TRUE if errors found in the program
 * @param is_external needs to store TRUE if external labels found in the program
 * @param is_entry needs to store TRUE if entry labels found in the program
 */
void remove_unnecessary_files(char* name_without_type, const char* error_found, char is_external, char is_entry);
/*printing functions*/
/**
 * prints instruction line words to machine code
 * word* needs to be given in the ... section, prints all of them.
 * @param machine_code to print to
 * @param line string line of the word
 * @param last_IC the IC of where the words start
 * @param num_of_words specify how many word are in the ... section
 * @param ... words pointers to print
 */
void print_code_words(FILE* machine_code, char* line, int last_IC, int num_of_words, ...);
/**
 * prints a label's name to the machine code with a '?' or "?&" before(in "jump to label")
 * in purpose of coding the label later
 * @param machine_code to print to
 * @param line the line of the label
 * @param word_to_print word of the label
 */
void print_label(FILE* machine_code, const char* line, word* word_to_print);
/**
 * prints the data image to machine code.
 * @param machine_code to print to
 * @param data data image
 * @param counters struct line_counters pointer
 */
void print_data(FILE* machine_code,data_image* data, line_counters* counters);
/**
 *prints the words count in the first line of the machine code(.ob)
 * @param machine_code to print to
 * @param counters struct line_counters pointer
 */
void print_words_count(FILE* machine_code, line_counters* counters);
/**
 * prints visual indication for the "report_error" function
 * @param index of the char of the error
 * @param line the string of the faulty line
 */
void print_visual_indication(int index, const char* line);
/**
 * prints the name and the address of the an external label to the .ext file
 * @param file to print to
 * @param external the external symbol
 * @param current_address string of the current address where the symbol is used
 */
void print_extern(FILE* file, symbol* external, char* current_address);
/**
 *prints the name and the address of the an entry label to the .ent file
 * @param file
 * @param entry
 * @param IC
 */
void print_entry(FILE* file, symbol* entry, int IC);
/**
 * prints an error summary. includes the number of errors.
 * @param file_name current file name to print it's name
 * @param errors_count to print
 */
void print_errors_summary(char* file_name, int errors_count);
/**/
/**
 * stops with exit code, optional - custom massage, if a massage isn't provided, ""(empty string) is required instead:
 * 1)with pre defined codes(below) and "" string (pre made massage to stderr).
 * 2)"EXIT_SUCCESS/FAILURE" and "" string(no massage to stderr).
 * "EXIT_SUCCESS/FAILURE" and a massage to stderr.
 * codes that are defined as macros:
 * MEMORY, FOPEN, EXIT_SUCCESS, EXIT_FAILURE.
 * @param exit_type
 * @param massage
 */
void stop(int exit_type, const char* massage);
#endif /*ASSEMBLER_IN_OUT_TOOLS_H*/
