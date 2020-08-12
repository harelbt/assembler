#ifndef ASSEMBLER_FIRST_PASS_H
#define ASSEMBLER_FIRST_PASS_H

#include "in out tools.h"
#include "assembler data types.h"
/*FUNCTIONS DECLARATION*/
symbol* first_pass(FILE* source, char* file_name,  FILE* machine_code, symbol** symbol_addresses_table, char* error_found);
static void free_first_pass(FILE* filep, line * sentence);
static void free_line(line* sentence);

/**/
#endif /*ASSEMBLER_FIRST_PASS_H*/
