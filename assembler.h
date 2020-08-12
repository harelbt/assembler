
#ifndef ASSEMBLER_ASSEMBLER_H
#define ASSEMBLER_ASSEMBLER_H
#include <stdlib.h>
#include "helpfunctions.h"
#include "first pass.h"
#define NUM_OF_ENGLISH_CHARS 52
#define TYPE_MAX_LENGTH 4
static FILE * open_machine_code(char* file_name);
static char* get_file_name_without_type(char* file_name);
static void remove_output_files(char* file_name);
static void remove_ob_file(char* file_name);
void remove_ent_file(char* file_name);
static void remove_ext_file(char* file_name);
#endif //ASSEMBLER_ASSEMBLER_H
