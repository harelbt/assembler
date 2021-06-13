#include <string.h>
#include <stdlib.h>
#include "first pass.h"
#include "line analyzer.h"
#include "errors.h"
#include "symbol table.h"
#include "translator.h"
/*
 * the first pass of the assembler.
 * 1)finds errors and reports to stdout.(using errors.c)
 * 2)calculates IC and DC
 * 3)translates the source file to a semi ready machine code.(using translator.c)
 * at the first pass stage, labels are printed in ?LABEL format(if the label's name is LABEL).
 * if label's distance is required, prints ?&LABEL.
 * 4) prepares the symbol table (using symbol table.c)
 * if errors found, turns "error_found" to TRUE, else FALSE
 */
void first_pass(FILE* source, FILE* machine_code, symbol* symbol_table, line_counters* counters, char* error_found) {
    /*flag to know if it's the first symbol to know if to copy a new symbol to the head, or to link it*/
    char is_first_symbol = TRUE;
    /*structs*/
    line sentence;/*struct with line components like label name, opcode, functions code etc...*/
    line_indexes indexes;/*struct with all the relevant indexes of the line components for the first pass*/
    data_image* data = allocate_memory(ONE_UNIT, DATA_IMAGE);/*head of the data image(linked list)*/
    (data->word.word) = 0;/*initializing bitfield to use it properly with masks*/
    data->next = NULL;/*for linked list behaviour*/
    data->is_head_filled = FALSE;/*flag to know if head is full, or needs to be filled*/
    data->DC = counters->DC;/*to initialize*/
    /*initializing counters with a life of a full first pass, counters of a line resets at every iteration below*/
    counters->line_number = 0;
    counters->error_number = 0;
    counters->IC = 100;
    counters->last_IC = 100;
    counters->DC = 0;
    counters->last_DC = 0;
    /*every iteration treats an assembly line*/
    while (!read_line(source, &sentence)) {/*"read_line" returns 0 at EOF(stops at EOF)*/
        /*resets the variables for each line*/
        initialize_line_variables(&sentence, counters, &indexes);
        /*sets the line number (starts at 0)*/
        counters->line_number++;
        /**/
        /*'\n' or '\r' line is a line we want to count but not to analyze("read_line" can't skip '\n' because it needs to stop at the end of the line)*/
        if (strcmp(sentence.line, "\n") != 0 && strcmp(sentence.line, "\r") != 0) {
            analyze_sentence(&sentence, &indexes, counters);/*parsing*/
            empty_or_comment_line_check(&sentence, &indexes);/*checks if comment or empty*/
            /*continues if not a comment /empty line*/
                if (sentence.flags.is_comment == FALSE && sentence.flags.is_empty_line == FALSE) {
                    /*inspects line for errors, if errors were found turns error_found flag to TRUE*/
                    if (errors_inspection(&sentence, &indexes, counters) == TRUE){
                        *error_found = TRUE;
                    }
                    if(*error_found == FALSE){/*if no errors found, continues first pass*/
                        calculate_number_of_words(&sentence, indexes, counters);/*calculates IC and DC*/
                        update_symbol_address(&sentence, *counters);/*if a label exists, sets its address*/
                        if (*sentence.label.name != '\0') {/*if a label exists, inserts it to the symbol table*/
                            symbol_table = insert_symbol(&sentence.label, symbol_table, &is_first_symbol);
                        }
                        first_pass_translation(machine_code, &sentence, &indexes, counters, data);/*translating*/
                    }
                }
        }
        free_line(&sentence);/*freeing struct line ("sentence")*/
    }
    /*prints data image to the machine code, so the data comes at the end*/
    print_data(machine_code, data, counters);
}
/*freeing struct line*/
void free_line(line* sentence){
    free(sentence->line);
    sentence->line = NULL;
}
