
#ifndef ASSEMBLER_ERRORS_H
#define ASSEMBLER_ERRORS_H
#define UNEXPECTED_SEMICOLON 1
#define UNEXPECTED_HASHMARK 2
#define UNEXPECTED_REGISTER 3
#define UNEXPECTED_QUOT_MARK 4
#include "assembler data types.h"

void report_error(line* sentence, short int error_code, error* error_list);
static void insert_error(error* error_list, char* message, int line, int char_number);
void output_errors(error* error_list);
static void create_error(error* new_list, char* message, int line, int char_number);
void insert_node(error* error_list, error* new_error);
#endif /*ASSEMBLER_ERRORS_H*/
