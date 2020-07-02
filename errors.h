
#ifndef ASSEMBLER_ERRORS_H
#define ASSEMBLER_ERRORS_H
#define UNEXPECTED_SEMICOLON 1
#include "assembler data.h"
typedef struct error_message{
    char* error;
    int line;
    struct error_message* next;
}error;

error* report_error(line* sentence, short int error_code, error* error_list);
void insert_error(error* error_list, char* message, int line);
void output_errors(error* error_list);
static void create_error(error* new_list, char* message, int line);
#endif /*ASSEMBLER_ERRORS_H*/
