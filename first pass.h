#ifndef ASSEMBLER_FIRST_PASS_H
#define ASSEMBLER_FIRST_PASS_H
#define operator unsigned int: 6
#define function unsigned int: 5
#define register_start unsigned int: 3
#define register_destination unsigned int: 3
#define mioon_start  unsigned int: 2
#define mioon_destination  unsigned int: 2
#define A unsigned int: 1
#define R unsigned int: 1
#define E unsigned int: 1
#define negate_to_binary(a)\
    a = (~a) +1;

#include "helpfunctions.h"
/*FUNCTIONS DECLARATION*/
symbol* first_pass(char * file, FILE* machine_code, symbol** symbol_addresses_table, char* error_found);
static void free_first_pass(FILE* filep, line * sentence);
static void free_line(line* sentence);

/**/
#endif /*ASSEMBLER_FIRST_PASS_H*/
