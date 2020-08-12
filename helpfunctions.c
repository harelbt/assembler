#include "helpfunctions.h"
static void invert_short_arr(short int*, int);
/*returns the first char after the white chars*/
int skip_white_chars(FILE* file){
    int ch;
    /*_____________________________________________________________*/
    while ((ch = fgetc(file)) == '\n' || ch == ' ' || ch == '\t');/*skipping engine*/
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

int current_word_length(const char* line, int index){
    char* string = allocate_arr_memory(1, CHAR);/*initial allocation*/
    int i = index;
    /*_____________________________________________________________*/
    while (*(line+i) != ' ' && *(line+i) != '\t' && *(line+i) != ',' && *(line+i)){
        i++;
    }
    /*_____________________________________________________________*/
    return i - index;
}
char* get_line_no_spaces(FILE* file){
    char* string = allocate_arr_memory(1, CHAR);/*initial allocation*/
    char* p = string;
    int i = 0;
    int ch;
    /*_____________________________________________________________*/
    while ((ch = fgetc(file)) != '\n' && ch != EOF){/*gets characters*/
        if (ch != ' ') {
            *(p + i) = (char) ch;
            i++;
            string = realloc_arr_memory(string, (i + 1), CHAR);/*expanding string*/
        }
    }
    /*_____________________________________________________________*/
    return string;
}
char* get_next_word(FILE* file){
    char* string = allocate_arr_memory(2, CHAR);/*initial allocation*/
    int ch = skip_white_chars(file);/*gets first char after white characters*/
    int i =0;
    /*_____________________________________________________________*/
    if (ch == EOF){/*no word found*/
        return "";
    }
    string[0] = (char) ch;/*enters first non white character*/
    i++;/*gets ready for the next character*/
    string = realloc_arr_memory(string,(i + 2),CHAR);/*expanding string*/
    while ((ch = fgetc(file)) != '\n' && ch != ' ' && ch != EOF && ch != '\t'){/*gets characters*/
        string[i] = (char) ch;
        i++;
        string = realloc_arr_memory(string,(i + 1),CHAR);/*expanding string*/
    }
    string[i] = '\0';
    /*_____________________________________________________________*/
    return string;
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
        str = allocate_arr_memory(count + 2, CHAR);
        str[count+1] = '\0';
        count++;/*starts ahead to save place for '-'*/
    } else {
        str = allocate_arr_memory(count + 1, CHAR);
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
    char* char_array = allocate_arr_memory((int)strlen(original_array),CHAR);
    /*_____________________________________________________________*/
    strcpy(char_array, original_array);
    /*_____________________________________________________________*/
    return char_array;
}
/**/
char* short_arr_to_str(short int* short_array, long length_or_end_number, int space_mode, int end_number_mode){
    char* str = allocate_arr_memory(1,CHAR);
    char* one_number;
    long i = 0;
    int length = 0;
    str[0] = '\0';
    while ((end_number_mode == OFF && i < length_or_end_number) || (end_number_mode == ON && short_array[i] != length_or_end_number)){
        one_number = decimal_to_str(short_array[i]);
        length = (int)(strlen(str)+strlen(one_number)+1);
        str = realloc_arr_memory(str,length, CHAR);
        strcat(str,one_number);
        free(one_number);
        i++;
        if (space_mode == ON){
            length =  (int)(strlen(str))+2;
            str = realloc_arr_memory(str,length, CHAR);
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
    char* str = allocate_arr_memory(1, CHAR);
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
        character = allocate_arr_memory(count + 1, CHAR);
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
char* get_line_dynamic(FILE* file, int* length){
    char* str = allocate_arr_memory(2, CHAR);
    int ch = skip_spaces(file);
    *length = 1;
    if (ch == EOF)
        return "";
    if (ch == '\n'){
        return "\n";
    }
    *(str) = (char) ch;
    while ((ch = fgetc(file)) != '\n' && ch != EOF){
        str = realloc_arr_memory(str,*length +2, CHAR);
        *(str + (*length)) = (char) ch;
        (*length)++;
    }
    *(str + (*length)) = '\0';
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
int find_first_char(line* sentence){
    int i = 0;
    while (sentence->line[i]){
        if (sentence->line[i] != ' ' && sentence->line[i] != '\t' && sentence->line[i] != '\n')
            return i;
        i++;
    }
    return -1;
}
int find_semicolon(line* sentence){
    int i = 0;
    while (sentence->line[i]){
        if (sentence->line[i] == ';') {
            return i;
        }
        i++;
    }
    return -1;
}
int read_line(FILE* file, line* sentence){
    sentence->line = get_line_dynamic(file, &sentence->length);
    if (!strcmp(sentence->line, ""))
        return EOF;
    return 0;
}


/*void find_label(line* sentence, line_marks_index indexes){
    if (indexes.colon_index >= 0){

    }
}*/
int find_next_space(const char* line, int index){
    char curr_char = *(line + index);
    int str_length = (int) strlen(line);
    int i = index;
    while (i < str_length && curr_char != ' ' && curr_char != '\t'){
        i++;
        curr_char = *(line + i);
    }
    return i;
}
int find_data_or_operands(const char* line, int index){
    index = find_next_space(line, index);
    index = find_next_word(line, index);
    return index;
}

