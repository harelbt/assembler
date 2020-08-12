#include <string.h>
#include <stdlib.h>
#include "in out tools.h"
char* get_line_dynamic(FILE* file, int* length){
    char* str = allocate_arr_memory(LINE_ASSUMED_LENGTH, CHAR);
    int ch = skip_spaces(file);
    int line_length_multiplier = 1;
    *(str + LINE_ASSUMED_LENGTH - 1) = '\0';
    *length = 1;
    if (ch == EOF)
        return "";
    if (ch == '\n'){
        return "\n";
    }
    *(str) = (char) ch;
    while ((ch = fgetc(file)) != '\n' && ch != EOF){
        if (*(str + (*length)) == '\0') {
            line_length_multiplier++;
            str = realloc_arr_memory(str, LINE_ASSUMED_LENGTH * line_length_multiplier, CHAR);
        }
        *(str + (*length)) = (char) ch;
        (*length)++;
    }
    if (*(str + (*length)) != '\0') {
        *(str + (*length)) = '\0';
        free((str + (*length) + 1));
    }
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
void* allocate_arr_memory (int size, char type){
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
void* realloc_arr_memory (void* ptr, int size, char type){
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
            case INPUT: {
                fputs("Can't open input file. Terminating program\n", stderr);
                exit(EXIT_FAILURE);
            }
            case OUTPUT: {
                fputs("Can't open output file. Terminating program\n", stderr);
                exit(EXIT_FAILURE);
            }
            case MEMORY: {
                fputs("Not enough memory\nTerminating program\n", stderr);
                exit(EXIT_FAILURE);
            }
            case FOPEN:{
                fputs("Can't open file. Terminating program\n", stderr);
                exit(EXIT_FAILURE);
            }
            case OTHER: {
                fputs("Something went wrong\nTerminating program\n", stderr);
                exit(EXIT_FAILURE);
            }
            case EXIT_SUCCESS: {
                exit(EXIT_SUCCESS);
            }
            case EXIT_FAILURE: {
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
    char* string = allocate_arr_memory(1, CHAR);/*initial allocation*/
    int i = index;
    int k = 0;
    /*_____________________________________________________________*/
    while (*(line + i) != ' ' && *(line + i) != '\t' && *(line + i)){
        *(string+k) = *(line+i);
        i++;
        k++;
        string = realloc_arr_memory(string, (k + 1), CHAR);/*expanding string*/
    }
    *(string+k) = '\0';
    /*_____________________________________________________________*/
    return string;
}