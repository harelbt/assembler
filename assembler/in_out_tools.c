#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "in_out_tools.h"
/*strings functions*/
/*
 *  gets a line from a file using dynamic memory allocation. returns a pointer to a string (char array)
 */
char* get_line_dynamic(FILE* file, int* length) {
    /*initial allocated string to return*/
    char *str = allocate_memory(LINE_ASSUMED_LENGTH, CHAR);
    int curr_char = skip_spaces(file);/*gets from the first non space char*/
    /*if a line length exceeded LINE_ASSUMED_LENGTH, multiplies the allocation with this variable*/
    int line_length_multiplier = 1;
    /*inserts first char and checks it*/
    *length = 1;
    *(str) = (char) curr_char;
    /*returns an empty string (not dynamically allocated) if the line is EOF*/
    if (curr_char == EOF) {
        free(str);
        return "";
    }
    /*if the line is only '\n', returns '\n'*/
    if (curr_char == '\n') {
        *(str + 1) = '\0';
        return str;
    }
    /*gets the line*/
    while ((curr_char = fgetc(file)) != '\n' && curr_char != '\r' && curr_char != EOF) {
        /*if a line length exceeded LINE_ASSUMED_LENGTH, multiplies the allocation with this variable*/
        if (*length == line_length_multiplier * LINE_ASSUMED_LENGTH) {
            line_length_multiplier++;
            str = realloc_memory(str, LINE_ASSUMED_LENGTH * line_length_multiplier, CHAR);
        }
        *(str + (*length)) = (char) curr_char;
        (*length)++;
    }
    /*on linux new line contain two chars, skips the second*/
    if (curr_char == '\r') {
        fgetc(file);
    }
    *(str + (*length)) = '\0';

    return str;
}
/*
 * reads a line from a file using dynamic memory allocation and stores it in a line*
 * it the line is EOF returns EOF, else returns 0;
 */
int read_line(FILE* file, line* sentence){
    /*gets line*/
    sentence->line = get_line_dynamic(file, &sentence->length);
    /*checks if EOF (see "get_line_dynamic" API)*/
    if (!strcmp(sentence->line, ""))
        return EOF;
    return 0;
}
/*returns the first char after the spaces. return value may be a new line character, consider using the skip_white_chars
 * function.*/
int skip_spaces(FILE* file){
    int ch;
    /*_____________________________________________________________*/
    while ((ch = fgetc(file)) == ' ');/*skipping engine*/
    /*_____________________________________________________________*/
    return ch;
}
/*
 * given an index of a white char, returns the index of the first character of the next word
 */
int find_next_word(const char* line, int index){
    int str_length = (int)strlen(line);
    int i = index;
    while (i < str_length && (*(line + i) == ' ' || *(line + i) == '\t')){
        i++;
    }
    return i;
}
/*returns a string from the file pointer until a white char(dynamic memory allocation)
 * if the file pointer points at a white char, returns "" (empty string)*/
char* get_until_white_char(const char* line, int index) {
    char* string = allocate_memory(LINE_ASSUMED_LENGTH, CHAR);/*initial allocation*/
    int i = index;
    int k = 0;
    int line_length_multiplier = 1;
    *(string) = '\0';
    /*_____________________________________________________________*/
    while (*(line + i) && *(line + i) != ' ' && *(line + i) != '\t'){
        *(string+k) = *(line+i);
        i++;
        k++;
        if (k == line_length_multiplier* LINE_ASSUMED_LENGTH) {
            line_length_multiplier++;
            string = realloc_memory(string, LINE_ASSUMED_LENGTH * line_length_multiplier, CHAR);
            *(string + LINE_ASSUMED_LENGTH * line_length_multiplier - 1) = '\0';
        }
    }
    *(string + k) = '\0';
    /*_____________________________________________________________*/
    return string;
}
/*memory allocation functions*/
/*like malloc, but in case of a failure, gives a proper massage  to stderr and exit the program, and does the casting
 *and all the hard work, and works with all of the relevant data types in the program
 * USE MACROS: SYMBOL, CHAR for type argument if the macro is undefined returns NULL*/
void* allocate_memory (int size, char type){
    /*allocation*/
    if (type == SYMBOL) {
        symbol* p = (symbol*) malloc(sizeof(symbol) * size);
        POINTER_CHECK
        return p;
    }
    if (type == CHAR) {
        char* p = (char*) malloc(sizeof(char) * size);
        POINTER_CHECK
        return p;
    }
    if (type == DATA_IMAGE) {
        char* p = (char*) malloc(sizeof(data_image) * size);
        POINTER_CHECK
        return p;
    }
    /*_____________________________________________________________*/
    /*if the macro is undefined returns NULL*/
    return NULL;
}

/*like realloc, but in case of a failure, gives a proper massage to stderr and exit the program, and does the casting
 * and all the hard work, and works with all of the relevant data types in the program
 * USE MACROS: SYMBOL, CHAR for type argument if the macro is undefined returns NULL*/
void* realloc_memory (void* ptr, int size, char type){
    /*reallocation*/
    if (type == SYMBOL) {
        symbol * p = (symbol *) realloc(ptr, sizeof(symbol) * size);
        POINTER_CHECK
        return p;
    }
    if (type == CHAR) {
        char* p = (char *) realloc(ptr, sizeof(char) * size);
        POINTER_CHECK
        return p;
    }
    /*_____________________________________________________________*/
    /*if the macro is undefined returns NULL*/
    return NULL;
}
/*files functions*/

/*
 *  like fopen, but if the opening failed gives a proper massage to stderr and exit the program
 */
FILE* open_file(const char* file_name, const char* open_type){
    FILE* file_pointer;
    file_pointer = fopen(file_name, open_type);/*the opening*/
    /*_____________________________________________________________*/
    if (file_pointer == NULL) {/*fail check*/
        stop(FOPEN,"");
    }
    /*_____________________________________________________________*/
    return file_pointer;
}
/*
 * opens an .ob file that is known to be the machine code for the imaginary cpu
 * the given name needs to be without a file type (for example .exe), mode argument is like in fopen
 * handles opening errors
 */
FILE* open_machine_code(char* file_name, const char* mode){
    int to_open_length = (int)strlen(file_name) + TYPE_MAX_LENGTH;
    /*to prepare the .ob file name*/
    char* to_open = allocate_memory(to_open_length, CHAR);
    FILE* machine_code;/*to return*/
    strcpy(to_open, file_name);/*copies the file name without the type*/
    strcat(to_open, ".ob\0");/*adds the type*/
    machine_code = open_file(to_open, mode);/*opens*/
    free(to_open);
    to_open = NULL;
    return machine_code;
}
/*
 * creates .ent file using a file name without file type
 */
FILE* create_ent_files(char* name_without_type){
    int ent_file_name_length = (int)strlen(name_without_type) + TYPE_MAX_LENGTH + 1;
    /*to prepare the .ent file name*/
    char* ent_file_name = allocate_memory(ent_file_name_length, CHAR);
    FILE* ent_file;
    strcpy(ent_file_name, name_without_type);/*copies the file name without the type*/
    strcat(ent_file_name, ".ent\0");/*adds the type*/
    ent_file = open_file(ent_file_name, "w+");/*opens*/
    free(ent_file_name);
    ent_file_name = NULL;
    return ent_file;
}
/*
 *creates .ext file using a file name without file type
 */
FILE* create_ext_files(char* name_without_type){
    int ext_file_name_length = (int)strlen(name_without_type) + TYPE_MAX_LENGTH + 1;
    /*to prepare the .ext file name*/
    char* ext_file_name = allocate_memory(ext_file_name_length, CHAR);
    FILE* ext_file;
    strcpy(ext_file_name, name_without_type);/*copies the file name without the type*/
    strcat(ext_file_name, ".ext\0");/*adds the type*/
    ext_file = open_file(ext_file_name, "w+");/*opens*/
    free(ext_file_name);
    ext_file_name = NULL;
    return ext_file;
}
/*
 * extracts the file name without the file type.
 * changes the original string
 */
char* get_file_name_without_type(char* file_name){
    /*gets the length, because the check of the string is from the end*/
    unsigned int length = strlen(file_name);
    char* i = file_name + length;/*pointer to the ende of the string*/
    /* steps back until the '.' of the type*/
    while (*i != '.'){
        i--;
    }
    /*cuts the type*/
    *i = '\0';
    return file_name;
}
/*
 * removes .ob .ent .ext files using a name without file type
 */
void remove_output_files(char* file_name){
    remove_ob_file(file_name);
    remove_ent_file(file_name);
    remove_ext_file(file_name);
}
/*
 * removes "temp.exe" file(used in for making the machine code)
 */
void remove_temp_file(){
    remove("temp.TXT");
}
/*
 * removes .ob file using a name without file type
 */
void remove_ob_file(char* file_name){
    int to_remove_length = (int)strlen(file_name) + TYPE_MAX_LENGTH;
    /*to prepare the .ob file name*/
    char* to_remove = allocate_memory(to_remove_length, CHAR);
    strcpy(to_remove, file_name);/*copies the file name without the type*/
    strcat(to_remove, ".ob\0");/*adds the type*/
    remove(to_remove);/*removes*/
    free(to_remove);
    to_remove = NULL;
}
/*
 * removes .ent file using a name without file type
 */
void remove_ent_file(char* file_name){
    int to_remove_length = (int)strlen(file_name) + TYPE_MAX_LENGTH + 1;
    /*to prepare the .ent file name*/
    char* to_remove = allocate_memory(to_remove_length, CHAR);
    strcpy(to_remove, file_name);/*copies the file name without the type*/
    strcat(to_remove, ".ent\0");/*adds the type*/
    remove(to_remove);/*removes*/
    free(to_remove);
    to_remove = NULL;
}
/*
 * removes .ext file using a name without file type
 */
void remove_ext_file(char* file_name){
    int to_remove_length = (int)strlen(file_name) + TYPE_MAX_LENGTH + 1;
    /*to prepare the .ent file name*/
    char* to_remove = allocate_memory(to_remove_length, CHAR);
    strcpy(to_remove, file_name);/*copies the file name without the type*/
    strcat(to_remove, ".ext\0");/*adds the type*/
    remove(to_remove);/*removes*/
    free(to_remove);
    to_remove = NULL;
}
/*
 * checks which files are unnecessary and removes them(.ob, .ext, .ent)
 * error_found needs to store TRUE if errors found in the program
 * is_external needs to store TRUE if external labels found in the program
 * is_entry needs to store TRUE if entry labels found in the program
 */
void remove_unnecessary_files(char* name_without_type, const char* error_found, const char is_external, const char is_entry){
    if (*error_found == TRUE) {
        remove_output_files(name_without_type);
    } else{
        if (is_external == FALSE){
            remove_ext_file(name_without_type);
        }
        if (is_entry == FALSE){
            remove_ent_file(name_without_type);
        }
    }
}
/*printing functions*/
/*
 * prints instruction line words to machine code.
 * word* needs to be given in the ... section, prints all of them.
 */
void print_code_words(FILE* machine_code, char* line, int last_IC, int num_of_words, ...) {
    int i = num_of_words;
    va_list arg_pointer;
    word *word_to_print;
    va_start(arg_pointer, num_of_words);
    while (i) {/*while there are word to print*/
        PRINT_ADDRESS;
        word_to_print = va_arg(arg_pointer, word*);
        /*if the word is a label, prints the label name to the file*/
        if (word_to_print->is_label == TRUE || word_to_print->is_jump == TRUE){
            print_label(machine_code, line, word_to_print);
        } else {/*prints prepared word*/
            unsigned int to_print = word_to_print->word;
            PRINT_CODE_WORD;
        }
        i--;
        last_IC++;/*to know the current address*/
    }
}
/*
 * prints a label's name to the machine code with a '?' or "?&" before(in "jump to label")
 * in purpose of coding the label later
 */
void print_label(FILE* machine_code, const char* line, word* word_to_print) {
    /*gets the string  from the start of the label*/
    char *label_name = get_until_white_char(line, word_to_print->label_index);
    unsigned int label_length = strlen(label_name);/*for loop boundaries*/
    /*to calculate the number of spaces to print to save space for hex print*/
    unsigned int i = label_length;
    PRINT_LABEL;/*the & in the "jumps labels" is in the label's name*/
    /*calculates the number of chars that the print captures,
     * calculates the number of spaces to print to save space for hex print*/
    if (label_length < HEX_PRINT_LENGTH) {
        i++;/*the '?' counts*/
        if (word_to_print->is_jump == TRUE){
            i++;/*the '&' counts*/
        }
        /*print spaces to save space for hex print*/
        while (i < HEX_PRINT_LENGTH) {
            fprintf(machine_code, " ");
            i++;
        }
    }
    fprintf(machine_code, "\n");
    free(label_name);
    label_name = NULL;
}
/*
 * prints the data image to machine code.
 */
void print_data(FILE* machine_code, data_image* data, line_counters* counters){
    data_image* data_pointer = data;
    unsigned int data_to_print;
    while (data->next != NULL){/*prints the data image*/
        data_to_print =  data->word.word;
        PRINT_DATA_WORD;
        data = data->next;
        free(data_pointer);
        data_pointer = data;
    }
    /*prints last word*/
    data_to_print =  data->word.word;
    PRINT_LAST_DATA_WORD;
    free(data);
}
/*
 * prints the words count in the first line of the machine code(.ob)
 */
void print_words_count(FILE* machine_code, line_counters* counters){
    fprintf(machine_code,"%d %d\n", counters->IC-100, counters->DC);
}
/*
 * prints visual indication for the "report_error" function
 */
void print_visual_indication(int index, const char* line) {
    int i = index;
    if (i == 1) {
        puts("_^");
        return;
    }
    while (i > 1) {
        putchar('_');
        i--;
    }
    puts("^");
    printf("Starting at the character ' %c '\n", *(line + index));
}
/*
 * prints the name and the address of the an external label to the .ext file
 */
void print_extern(FILE* file, symbol* external, char* current_address){
    int address = (int)strtod(current_address, NULL);
    fprintf(file, "%s ", external->name);
    fprintf(file, "%07d\n", address);
}
/*
 * prints the name and the address of the an entry label to the .ent file
 */
void print_entry(FILE* file, symbol* entry, int IC){
    int address = entry->address;
    /*order lines has IC address addition*/
    if (entry->sentence_type == DATA){
        address += IC;
    }
    fprintf(file, "%s ", entry->name);
    fprintf(file, "%07d\n", address);
}
/*
 * prints an error summary. includes the number of errors.
 */
void print_errors_summary(char* file_name, int errors_count) {
    printf("\nFILE: %s\n", file_name);
    printf("%d ERRORS WAS FOUND\n", errors_count);
    puts("NO OUTPUT FILES WERE GENERATED"
         "\n*************************************************************************************");
}
/*stops with exit code.
 * 1)with pre defined codes and "" string (pre made massage to stderr).
 * 2)"EXIT_SUCCESS/FAILURE" and "" string(no massage to stderr).
 * "EXIT_SUCCESS/FAILURE" and a massage to stderr.
 * codes were defined as macros*/
void stop(int exit_type, const char* to_print) {
    /*custom exit with a given string*/
    if (strcmp(to_print, "") != 0 && (exit_type == EXIT_FAILURE || exit_type == EXIT_SUCCESS)) {
        fputs(to_print, stderr);
        exit(exit_type);
    } else/*exit with exit code and "" (empty) string*/
        switch (exit_type) {
            case MEMORY: {
                fputs("Not enough memory\nTerminating program\n", stderr);
                exit(EXIT_FAILURE);
            }
            case FOPEN: {
                fputs("Can't open file. Terminating program\n", stderr);
                exit(EXIT_FAILURE);
            }
            default:
                exit(EXIT_FAILURE);
        }
}