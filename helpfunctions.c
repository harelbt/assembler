/*
 Created by Harel Bentabou on 07/06/2020.
 Update  08/06/2020: new funcs: 1)str_to_binary 2)int_to_str 3)invert_short_arr(private / static).
*/
#include "helpfunctions.h"

static void invert_short_arr(short int*, int);
/*returns the first char after the white chars*/
int skip_white_chars(FILE* file){
    int ch;
    /*_____________________________________________________________*/
    while ((ch = fgetc(file)) == '\n' || ch == ' ');/*skipping engine*/
    /*_____________________________________________________________*/
    return ch;
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
/*allocates memory for array of any type safely and exits program in case of failure with a proper massage to stderr
 * returns the address*/
void* allocate_arr_memory (int size, const char* type){
    /*allocation*/
    if (!strcmp(type,"symbol")) {
        symbol * p = (symbol *) malloc(sizeof(symbol) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type,"char")) {
        char* p = (char *) malloc(sizeof(char) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "unsigned char")) {
        unsigned char* p = (unsigned char *) malloc(sizeof( char) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "signed char")) {
        signed char* p = (signed char *) malloc(sizeof(char) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "short")) {
        short* p = (short*) malloc(sizeof(short) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "short int")) {
        short int* p = (short int*) malloc(sizeof(short int) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "signed short")) {
        signed short* p = (signed short*) malloc(sizeof(signed short) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "signed short int")) {
        signed short int* p = (signed short int*) malloc(sizeof(signed short int) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "unsigned short")) {
        unsigned short* p = (unsigned short*) malloc(sizeof(unsigned short) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "unsigned short int")) {
        unsigned short int* p = (unsigned short int*) malloc(sizeof(unsigned short int) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "int")) {
        int* p = (int*) malloc(sizeof(int) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "signed")) {
        signed * p = (signed*) malloc(sizeof(signed) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "signed int")) {
        signed int* p = (signed int*) malloc(sizeof(signed int) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "unsigned")) {
        unsigned * p = (unsigned*) malloc(sizeof(unsigned) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "unsigned int")) {
        unsigned int* p = (unsigned int*) malloc(sizeof(unsigned int) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "long")) {
        long* p = (long*) malloc(sizeof(long) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "long int")) {
        long int* p = (long int*) malloc(sizeof(long int) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "signed long")) {
        signed long* p = (signed long*) malloc(sizeof(signed long) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "signed long int")) {
        signed long int* p = (signed long int*) malloc(sizeof(signed long int) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "unsigned long")) {
        unsigned long* p = (unsigned long*) malloc(sizeof(unsigned long) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "unsigned long int")) {
        unsigned long int* p = (unsigned long int*) malloc(sizeof(unsigned long int) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "float")) {
        float* p = (float*) malloc(sizeof(float) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "double")) {
        double* p = (double*) malloc(sizeof(double) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "long double")) {
        long double* p = (long double*) malloc(sizeof(long double) * size);
        POINTER_CHECK
        return p;
    }
    /*_____________________________________________________________*/
    return NULL;
}
/*reallocates memory for array of any type safely and exits program in case of failure with a proper massage to stderr
 * returns the address*/
void* realloc_arr_memory (void* ptr, int size, const char* type){
    /*reallocation*/
    if (!strcmp(type,"symbol")) {
        symbol * p = (symbol *) realloc(ptr, sizeof(symbol) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type,"char")) {
        char* p = (char *) realloc(ptr, sizeof(char) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "unsigned char")) {
        unsigned char* p = (unsigned char *) realloc(ptr, sizeof( char) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "signed char")) {
        signed char* p = (signed char *) realloc(ptr, sizeof(char) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "short")) {
        short* p = (short*) realloc(ptr, sizeof(short) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "short int")) {
        short int* p = (short int*) realloc(ptr, sizeof(short int) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "signed short")) {
        signed short* p = (signed short*) realloc(ptr, sizeof(signed short) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "signed short int")) {
        signed short int* p = (signed short int*) realloc(ptr, sizeof(signed short int) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "unsigned short")) {
        unsigned short* p = (unsigned short*) realloc(ptr, sizeof(unsigned short) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "unsigned short int")) {
        unsigned short int* p = (unsigned short int*) realloc(ptr, sizeof(unsigned short int) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "int")) {
        int* p = (int*) realloc(ptr, sizeof(int) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "signed")) {
        signed * p = (signed*) realloc(ptr, sizeof(signed) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "signed int")) {
        signed int* p = (signed int*) realloc(ptr, sizeof(signed int) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "unsigned")) {
        unsigned * p = (unsigned*) realloc(ptr, sizeof(unsigned) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "unsigned int")) {
        unsigned int* p = (unsigned int*) realloc(ptr, sizeof(unsigned int) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "long")) {
        long* p = (long*) realloc(ptr, sizeof(long) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "long int")) {
        long int* p = (long int*) realloc(ptr, sizeof(long int) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "signed long")) {
        signed long* p = (signed long*) realloc(ptr, sizeof(signed long) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "signed long int")) {
        signed long int* p = (signed long int*) realloc(ptr, sizeof(signed long int) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "unsigned long")) {
        unsigned long* p = (unsigned long*) realloc(ptr, sizeof(unsigned long) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "unsigned long int")) {
        unsigned long int* p = (unsigned long int*) realloc(ptr, sizeof(unsigned long int) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "float")) {
        float* p = (float*) realloc(ptr, sizeof(float) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "double")) {
        double* p = (double*) realloc(ptr, sizeof(double) * size);
        POINTER_CHECK
        return p;
    }
    if (!strcmp(type, "long double")) {
        long double *p = (long double *) realloc(ptr, sizeof(long double) * size);
        POINTER_CHECK
        return p;
    }
    /*_____________________________________________________________*/
    return NULL;
}
/*returns a string from the file pointer until a white char(dynamic memory allocation)
 * if the file pointer points at a white char, returns "" (empty string)*/
char* get_until_white_char(FILE* file) {
    char* string = allocate_arr_memory(1, "char");/*initial allocation*/
    char* p = string;
    int i = 0;
    int ch;
    /*_____________________________________________________________*/
    while ((ch = fgetc(file)) != '\n' && ch != ' ' && ch != EOF){/*gets characters*/
        *(p+i) = (char) ch;
        i++;
        string = realloc_arr_memory(string,(i + 1),"char");/*expanding string*/
    }
    /*_____________________________________________________________*/
    return string;
}
char* get_line_no_spaces(FILE* file){
    char* string = allocate_arr_memory(1, "char");/*initial allocation*/
    char* p = string;
    int i = 0;
    int ch;
    /*_____________________________________________________________*/
    while ((ch = fgetc(file)) != '\n' && ch != EOF){/*gets characters*/
        if (ch != ' ') {
            *(p + i) = (char) ch;
            i++;
            string = realloc_arr_memory(string, (i + 1), "char");/*expanding string*/
        }
    }
    /*_____________________________________________________________*/
    return string;
}
char* get_next_word(FILE* file){
    char* string = allocate_arr_memory(2, "char");/*initial allocation*/
    char* p = string;
    int ch = skip_white_chars(file);/*gets first char after white characters*/
    int i =0;
    /*_____________________________________________________________*/
    if (ch == EOF){/*no word found*/
        return "";
    }
    *p = (char) ch;/*enters first non white character*/
    i++;/*gets ready for the next character*/
    string = realloc_arr_memory(string,(i + 1),"char");/*expanding string*/
    while ((ch = fgetc(file)) != '\n' && ch != ' ' && ch != EOF){/*gets characters*/
        *(p+i) = (char) ch;
        i++;
        string = realloc_arr_memory(string,(i + 1),"char");/*expanding string*/
    }
    /*_____________________________________________________________*/
    return string;
}
/*translates strings to binary base
 * returns the result in a long array
 * every char's translation is stored in a different cell ("12" = |1|10|)
 * */
long* str_to_binary(const char* str) {
        long *translation = allocate_arr_memory(1, "long");/*minimum required size*/
        /*gets value from the arguments*/
        long binary = 0;
        long reminder;
        long i = 1;
        long j = 0;
        /*_____________________________________________________________*/
        while (str[j] != '\0') {
            /*copying char*/
            translation[j] = (long) str[j];
            /*translating char*/
            while (translation[j] != 0) {
                reminder = translation[j] % 2;
                translation[j] /= 2;
                binary += reminder * i;
                i *= 10;
            }
            /*copying translation to the array*/
            translation[j] = binary;
            /*recalibrate variables*/
            binary = 0;
            i = 1;
            j++;
            /*redetermines size according to the index*/
            translation = realloc_arr_memory(translation, j + 1, "long");
        }
        /*puts -1 in the end*/
        translation[j] = -1;
        /*_____________________________________________________________*/
        return translation;
}
/*returns an int as a string (char*)*/
char* decimal_to_str(long number) {
    long number_copy = number;
    int count = 0;
    int negative = 0;
    char *str;
    /*_____________________________________________________________*/
    if (number < 0)
        negative = 1;
    if (number == 0){
        return "0";
    }
    /*counts "length"*/
    while (number_copy != 0) {
        count++;
        number_copy /= 10;
    }
    /*allocating the necessary memory*/
    if (negative == 1) {
        /*saves place for a '-'*/
        str = allocate_arr_memory(count + 2, "char");
        str[count+1] = '\0';
        count++;/*starts ahead to save place for '-'*/
    } else {
        str = allocate_arr_memory(count + 1, "char");
        str[count] = '\0';
    }
    /*enters values to the string*/
    while (count != 0) {
        /*gets last number*/
        number_copy = number % 10;
        number /= 10;
        /*updates the index*/
        count--;
        /*if the number is negative, adds a '-' to the start (cell pre saved)*/
        if (negative == 1 && count == 0) {
            str[count] = '-';
            break;
        }
        /*inserts accordingly*/
        insert_digit_to_str(str,number_copy,count);
    }
    /*_____________________________________________________________*/
    return str;
}

/*(private) inverting long arrays, ignores last cell*/
static void invert_short_arr(short int* array, int length){
    long i = 0;
    long j = length - 1;
    short int swap;
    /*_____________________________________________________________*/
    while (i < j){
        swap = array[i];
        array[i] = array[j];
        array[j] = swap;
        i++;
        j--;
    }
}
char* strUnconst(const char* original_array){
    char* char_array = allocate_arr_memory((int)strlen(original_array),"char");
    /*_____________________________________________________________*/
    strcpy(char_array, original_array);
    /*_____________________________________________________________*/
    return char_array;
}
/**/
char* short_arr_to_str(short int* short_array, long length_or_end_number, int space_mode, int end_number_mode){
    char* str = allocate_arr_memory(1,"char");
    char* one_number;
    long i = 0;
    int length = 0;
    str[0] = '\0';
    while ((end_number_mode == OFF && i < length_or_end_number) || (end_number_mode == ON && short_array[i] != length_or_end_number)){
        one_number = decimal_to_str(short_array[i]);
        length = (int)(strlen(str)+strlen(one_number)+1);
        str = realloc_arr_memory(str,length, "char");
        strcat(str,one_number);
        free(one_number);
        i++;
        if (space_mode == ON){
            length =  (int)(strlen(str))+2;
            str = realloc_arr_memory(str,length, "char");
            str[length-2] = ' ';
            str[length-1] = '\0';
        }
    }
    str[strlen(str)] = '\0';
    return str;
}

void insert_digit_to_str(char* str, int number, long index){
    switch (abs(number)) {
        case 0: {
            str[index] = '0';
            break;
        }
        case 1: {
            str[index] = '1';
            break;
        }
        case 2: {
            str[index] = '2';
            break;
        }
        case 3: {
            str[index] = '3';
            break;
        }
        case 4: {
            str[index] = '4';
            break;
        }
        case 5: {
            str[index] = '5';
            break;
        }
        case 6: {
            str[index] = '6';
            break;
        }
        case 7: {
            str[index] = '7';
            break;
        }
        case 8: {
            str[index] = '8';
            break;
        }
        case 9: {
            str[index] = '9';
            break;
        }
    }
}
long short_arr_length(const short int* array, int last_number){
    int i = 0;
    while (array[i] != last_number){
        i++;
    }
    i++;
    return i;
}
/*only sentences with a space between words, cant handle big numbers*/
char* binary_to_str(const char* to_translate){
    int i = 0;
    int j = 0;
    int k = 0;
    int count = 0;
    int mult_factor = 1;
    char* character;
    long long_character;
    short int* short_character_arr;
    char* str = allocate_arr_memory(1, "char");
    char* endp = NULL;
    long str_length = 1;
    int ascii = 0;
    long to_translate_length = (long) strlen(to_translate);
    while (i < to_translate_length) {
        while (to_translate[k] != ' ' && to_translate[k] != '\0') {
            count++;
            k++;
        }
        k++;
        character = allocate_arr_memory(count + 1, "char");
        while (to_translate[i] != ' ' && to_translate[i] != '\0') {
            character[j] = to_translate[i];
            if (character[j] != '0' && character[j] != '1'){
                return "Not a binary code, try again";
            }
            i++;
            j++;
        }
        i++;
        character[j] = '\0';
        long_character = strtol(character, &endp, 10);
        free(character);
        if (long_character == 2147483647){
            return "The number you have entered is too big, try again";
        }
        short_character_arr = long_to_short_arr(long_character);
        invert_short_arr(short_character_arr, count);
        j = 0;
        while (j < count) {
            ascii += short_character_arr[j] * mult_factor;
            mult_factor *= 2;
            j++;
        }
        free(short_character_arr);
        str = realloc_arr_memory(str, str_length + 1, "char");
        str[str_length-1] = (char) ascii;
        str_length++;
        j = 0;
        count = 0;
        mult_factor = 1;
        ascii = 0;
    }
    str[str_length - 1] = '\0';
    return str;
}
short int* long_to_short_arr(long number){
    long number_copy = number;
    short int* short_arr;
    int count = 0;
    int i = 0;
    while (number_copy != 0){
        number_copy /= 10;
        count++;
    }
    short_arr = allocate_arr_memory(count + 1, "long");
    while (i < count){
        short_arr[i] = number % 10;
        number /= 10;
        i++;
    }
    short_arr[i] = -1;
    invert_short_arr(short_arr, count);
    return short_arr;
}
char* get_line_dynamic(FILE* file){
    char* str = allocate_arr_memory(2, "char");
    int ch = skip_spaces(file);
    int length = 1;
    if (ch == EOF)
        return "";
    str[0] = (char) ch;
    while ((ch = fgetc(file)) != '\n' && ch != EOF){
        str = realloc_arr_memory(str,length +2, "char");
        str[length] = (char) ch;
        length++;
    }
    str[length] = '\0';
    return str;
}
int get_to_eof(FILE* file){
    int ch;
    while ((ch = fgetc(file)) != EOF){}
    return ch;
}
int get_line(FILE* file){
    int ch;
    while ((ch = fgetc(file)) != '\n' && ch != EOF){}
    return ch;
}
char four_binaries_to_hex (short int binary){
    switch (binary) {
        case 0: {
            return '0';
        }
        case 1: {
            return '1';
        }
        case 10: {
            return '2';
        }
        case 11: {
            return '3';
        }
        case 100: {
            return '4';
        }
        case 101: {
            return '5';
        }
        case 110: {
            return '6';
        }
        case 111: {
            return '7';
        }
        case 1000: {
            return '8';
        }
        case 1001: {
            return '9';
        }
        case 1010: {
            return 'a';
        }
        case 1011: {
            return 'b';
        }
        case 1100: {
            return 'c';
        }
        case 1101: {
            return 'd';
        }
        case 1110: {
            return 'e';
        }
        case 1111: {
            return 'f';
        }
        default:{
            return 'G';
        }

    }
}
short int* dec_to_binary(long number){
    short int* translation = allocate_arr_memory(1, "short int");/*minimum required size*/
    short int reminder;
    long curr_index = 0;
    short int negative;
    if (number < 0) {
        negative = 1;
        number += -2*number;
    } else {
        negative = 0;
    }
    while (number != 0) {/*every iteration finds the next 0 or 1*/
        reminder = number % 2;
        number /= 2;
        translation[curr_index] = reminder;
        curr_index++;
        /*redetermines size according to curr_index*/
        translation = realloc_arr_memory(translation, curr_index + 1, "short int");
    }
    /*-1 in the last cell*/
    translation[curr_index] = -1;
    /*array needs to be inverted*/
    invert_short_arr(translation, curr_index);
    if (negative == 1){
        comp_of_2(translation, -1);
    }
    /*_____________________________________________________________*/
    return translation;
}
short int* comp_of_2 (short int* binary,long length){
    if (length == -1){
        length = short_arr_length(binary, -1);
    }
    length -= 2;
    while (length > 0){
        if (binary[length-1] == 1){
            binary[length-1] = 0;
        } else
            binary[length-1] = 1;
        length--;
    }
    return binary;
}