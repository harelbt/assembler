#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "in out tools.h"
char* get_line_dynamic(FILE* file, int* length){
    char* str = allocate_memory(LINE_ASSUMED_LENGTH, CHAR);
    int ch = skip_spaces(file);
    int line_length_multiplier = 1;
    *length = 1;
    *(str) = (char) ch;
    if (ch == EOF) {
        free(str);
        return "";
    }
    if (ch == '\n'){
        *(str + 1) = '\0';
        return str;
    }
    while ((ch = fgetc(file)) != '\n' && ch != '\r' && ch != EOF){
        if (*length == line_length_multiplier*LINE_ASSUMED_LENGTH) {
            line_length_multiplier++;
            str = realloc_memory(str, LINE_ASSUMED_LENGTH * line_length_multiplier, CHAR);
        }
        *(str + (*length)) = (char) ch;
        (*length)++;
    }
    if (ch == '\r'){
        fgetc(file);/*on linux new line contain two chars, skips the second*/
    }
        *(str + (*length)) = '\0';

    return str;
}
int read_line(FILE* file, line* sentence){
    sentence->line = get_line_dynamic(file, &sentence->length);
    if (!strcmp(sentence->line, ""))
        return EOF;
    return 0;
}
/*allocates memory for array of any type safely and exits program in case of failure with a proper massage to stderr
 * returns the address*/
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
    return NULL;
}

/*reallocates memory for array of any type safely and exits program in case of failure with a proper massage to stderr
 * returns the address*/
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
    return NULL;
}
/*returns the first char after the spaces. return value may be a new line character, consider using the skip_white_chars
  function.*/
int skip_spaces(FILE* file){
    int ch;
    /*_____________________________________________________________*/
    while ((ch = fgetc(file)) == ' ');/*skipping engine*/
    /*_____________________________________________________________*/
    return ch;
}
/*stops with exit code.
 * 1)with pre defined codes and "" string (pre made massage to stderr).
 * 2)"EXIT_SUCCESS/FAILURE" and "" string(no massage to stderr).
 * "EXIT_SUCCESS/FAILURE" and a massage to stderr.
 * codes were defined as macros*/
void stop(int exit_type, const char* to_print) {
    /*custom exit with a given string*/
    if (strcmp(to_print,"") != 0 && (exit_type == EXIT_FAILURE || exit_type == EXIT_SUCCESS)) {
        fputs(to_print,stderr);
        exit(exit_type);
    } else/*exit with exit code and "" (empty) string*/
        switch (exit_type) {
            case MEMORY: {
                fputs("Not enough memory\nTerminating program\n", stderr);
                exit(EXIT_FAILURE);
            }
            case FOPEN:{
                fputs("Can't open file. Terminating program\n", stderr);
                exit(EXIT_FAILURE);
            }
            default: exit(EXIT_FAILURE);
        }
}
/*opens file safely and exits program in case of failure with a proper massage to stderr*/
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
char* get_file_name_without_type(char* file_name){
    unsigned int length = strlen(file_name);
    char* i = file_name + length;
    while (*i != '.'){
        i--;
    }
    *i = '\0';
    return file_name;
}
void remove_output_files(char* file_name){
    remove_ob_file(file_name);
    remove_ent_file(file_name);
    remove_ext_file(file_name);
}
void remove_ob_file(char* file_name){
    char to_remove [strlen(file_name) + TYPE_MAX_LENGTH];
    strcpy(to_remove, file_name);
    strcat(to_remove, ".ob\0");
    remove(to_remove);
}
void remove_ent_file(char* file_name){
    char to_remove [strlen(file_name) + TYPE_MAX_LENGTH + 1];
    strcpy(to_remove, file_name);
    strcat(to_remove, ".ext\0");
    remove(to_remove);
}
void remove_ext_file(char* file_name){
    char to_remove [strlen(file_name) + TYPE_MAX_LENGTH + 1];
    strcpy(to_remove, file_name);
    strcat(to_remove, ".ent\0");
    remove(to_remove);
}
FILE* open_machine_code(char* file_name){
    char to_open[strlen(file_name) + TYPE_MAX_LENGTH];
    FILE* machine_code;
    strcpy(to_open, file_name);
    strcat(to_open, ".ob\0");
    machine_code = open_file(to_open, "w+");
    return machine_code;
}
FILE* create_ent_files(char* name_without_type){
    char ent_file_name[strlen(name_without_type) + TYPE_MAX_LENGTH + 1];
    strcpy(ent_file_name, name_without_type);
    strcat(ent_file_name, ".ent\0");
    return open_file(ent_file_name, "w+");
}
FILE* create_ext_files(char* name_without_type){
    char ext_file_name[strlen(name_without_type) + TYPE_MAX_LENGTH + 1];
    strcpy(ext_file_name, name_without_type);
    strcat(ext_file_name, ".ext\0");
    return open_file(ext_file_name, "w+");
}
void print_entry_extern(FILE* file, symbol* entry_extern){
    fprintf(file, "%s ", entry_extern->name);
    fprintf(file, "%07d", entry_extern->address);
}
void remove_unnecessary_files(char* name_without_type, const char* error_found, const char* is_external, const char* is_entry){
    if (*error_found == TRUE) {
        remove_output_files(name_without_type);
    } else{
        if (*is_external == FALSE){
            remove_ext_file(name_without_type);
        }
        if (*is_entry == FALSE){
            remove_ent_file(name_without_type);
        }
    }
}
void print_code_words(FILE* machine_code, char* line, line_indexes* indexes, int last_IC, int num_of_words, ...) {
    int i = num_of_words;
    va_list arg_pointer;
    word *word_to_print;
    va_start(arg_pointer, num_of_words);
    while (i) {
        PRINT_ADDRESS;
                word_to_print = va_arg(arg_pointer, word*);
        if (word_to_print->is_label == TRUE || word_to_print->is_jump == TRUE){
            print_label(machine_code, line, word_to_print);
        } else {/*prints prepared word*/
            unsigned int to_print = word_to_print->word;
            PRINT_CODE_WORD;
        }
        i--;
        last_IC++;
    }
}
void print_label(FILE* machine_code, const char* line, word* word_to_print) {
    char *label_name = get_until_white_char(line, word_to_print->label_index);
    unsigned int label_length = strlen(label_name);
    unsigned int i = label_length;
    PRINT_LABEL;
    if (label_length < HEX_PRINT_LENGTH) {
        i++;/*the '?' counts*/
        if (word_to_print->is_jump == TRUE){
            i++;/*the '&' counts*/
        }
        while (i != HEX_PRINT_LENGTH) {
            fprintf(machine_code, " ");
            i++;
        }
    }
    fprintf(machine_code, "\n");
    free(label_name);
    label_name = NULL;
}
void print_data(FILE* machine_code, data_image* data, line_counters* counters){
    data_image* data_pointer = data;
    int data_print_counter = 1;
    unsigned int data_to_print;
    while (data->next != NULL){
        data_to_print =  data->word.word;
        PRINT_DATA_WORD;
        data = data->next;
        free(data_pointer);
        data_pointer = data;
        data_print_counter++;
    }
    data_to_print =  data->word.word;
    PRINT_DATA_WORD;
    free(data);
}