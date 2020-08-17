#include <string.h>
#include <stdlib.h>
#include "first pass.h"
#include "line analyzer.h"
#include "errors.h"
#include "symbol table.h"
#include "translator.h"
void first_pass(FILE* source, char* file_name, FILE* machine_code, symbol* symbol_table, line_counters* counters, char* error_found) {
    /*structs*/
    line sentence;
    line_indexes indexes;
    data_image* data = allocate_memory(1, DATA_IMAGE);
    (data->word.word) = 0;
    /**/
    counters->line_number = 0;
    counters->error_number = 0;
    counters->IC = 100;
    counters->last_instruction_address = 100;
    counters->DC = 0;
    counters->last_data_address = 0;
    data->next = NULL;
    data->is_head_filled = FALSE;
    /**/
    char is_first_symbol = TRUE;
    /**/
    while (!read_line(source, &sentence)) {/*"read_line" returns 0 at EOF*/
        /*resets the variables for each line*/
        initialize_line_tools(&sentence, counters, &indexes);
        /**/
        counters->line_number++;
        printf("%d\n", counters->line_number);
        /**/
        /*'\n' or '\r' line is a line we want to count but not to read("read_line" can't skip '\n' because it needs to stop at the end of the line)*/
        if (strcmp(sentence.line, "\n") != 0 && strcmp(sentence.line, "\r") != 0) {
            analyze_sentence(&sentence, &indexes, counters);/*parsing*/
            empty_or_comment_line_check(&sentence, &indexes);
            /*continues if not a comment /empty line*/
                if (sentence.flags.is_comment == FALSE && sentence.flags.is_empty_line == FALSE) {
                    if (errors_inspection(&sentence, &indexes, counters) == TRUE){
                        *error_found = TRUE;
                    }
                    if(*error_found == FALSE){/*if no errors found*/
                        calculate_number_of_words(&sentence, indexes, counters);
                        update_symbol_address(&sentence, *counters);
                        if (*sentence.label.name != '\0') {
                            symbol_table = insert_symbol(&sentence.label, symbol_table, &is_first_symbol, counters, error_found);
                        }

                        first_pass_translation(machine_code, &sentence, &indexes, counters, data);
                    }
                }
        }
        free_line(&sentence);
    }
    print_data(machine_code, data, counters);
    if (*error_found == TRUE) {
        print_errors_summary(file_name, counters->error_number);
    }
}
static void free_line(line* sentence){
    free(sentence->line);
    sentence->line = NULL;
}