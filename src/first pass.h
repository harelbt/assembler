#ifndef ASSEMBLER_FIRST_PASS_H
#define ASSEMBLER_FIRST_PASS_H

#include "in_out_tools.h"
#include "assembler data types.h"

/*FUNCTIONS DECLARATION*/
/**
 * the first pass of the assembler.
 * 1)finds errors and reports to stdout.(using errors.c)
 * 2)calculates IC and DC
 * 3)translates the source file to a semi ready machine code.(using translator.c)
 * at the first pass stage, labels are printed in ?LABEL format(if the label's name is LABEL).
 * if label's distance is required, prints ?&LABEL.
 * 4) prepares the symbol table (using symbol table.c)
 * if errors found, turns "error_found" to TRUE, else FALSE
 * @param source FILE* input assemebly code file
 * @param FILE* machine_code file to write the first pass translation to.
 * @param symbol_table pointer to symbol_table head
 * @param counters struct line_counters
 * @param error_found if errors found, turns "error_found" to TRUE, else FALSE
 */
void first_pass(FILE* source, FILE* machine_code, symbol* symbol_table, line_counters * counters, char* error_found);
/**
 * freeing struct line
 * @param sentence struct line
 */
void free_line(line* sentence);
/**/
#endif /*ASSEMBLER_FIRST_PASS_H*/
