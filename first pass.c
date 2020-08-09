#include "first pass.h"
#include "line analyzer.h"
#include "errors.h"
#include "words.h"
#include "symbol table.h"
#include "translator.h"
symbol* first_pass(FILE* source, FILE* machine_code, symbol** symbol_addresses_table, char* error_found) {
    /*structs*/
    line sentence;
    line_indexes indexes;
    line_counters counters;
    symbol* symbol_table = allocate_arr_memory(1, SYMBOL);
    data_image* data = allocate_arr_memory(1, DATA_IMAGE);
    /**/
    counters.line_number = 0;
    counters.error_number = 0;
    counters.IC = 100;
    counters.last_instruction_address = 100;
    counters.DC = 0;
    counters.last_data_address = 0;
    data->next = NULL;
    data->is_head_filled = FALSE;
    /**/
    char is_first_symbol = TRUE;
    /**/
    while (!read_line(source, &sentence)) {/*"read_line" returns 0 at EOF*/
        /*resets the variables for each line*/
        initialize_line_tools(&sentence, &counters, &indexes);
        /**/
        counters.line_number++;
        /**/
        /*'\n' line is a line we want to count but not to read("read_line" can't skip '\n' because it needs to stop at the end of the line)*/
        if (strcmp(sentence.line, "\n") != 0) {
            analyze_sentence(&sentence, &indexes, &counters);/*parsing*/
            empty_or_comment_line_check(&sentence, &indexes);
            /*continues if not a comment/empty line*/
                if (sentence.flags.is_comment == FALSE && sentence.flags.is_empty_line == FALSE) {
                    if (errors_inspection(&sentence, &indexes, &counters) == TRUE){
                        *error_found = TRUE;
                    }
                    if(*error_found == FALSE){/*if no errors found*/
                        calculate_number_of_words(&sentence, indexes, &counters);
                        update_symbol_address(&sentence, counters);
                        if (*sentence.label.name != '\0') {
                            symbol_table = insert_symbol(&sentence.label, symbol_addresses_table, symbol_table, &is_first_symbol, &counters, error_found);
                        }
                        first_pass_translation(machine_code, &sentence, &indexes, &counters, data);
                    }
                }
        }
    }
    while (data->next != NULL){
        unsigned int a =  data->word.word;
        printf("%d %x\n", data->DC,a);
        data = data->next;
    }
    printf("%d %x\n", data->DC, data->word.word);
    if (*error_found == TRUE) {
        print_errors_summary(file, counters.error_number);
    }
    free_first_pass(source, &sentence);
    return symbol_table;
}

static void free_first_pass(FILE* filep, line* sentence){
    fclose(filep);
    free(filep);
    filep = NULL;
    free_line(sentence);
}
static void free_line(line* sentence){
    free(sentence->line);
    sentence->line = NULL;
    if (sentence->data_parts.data != NULL){
        free(sentence->data_parts.data);
        sentence->data_parts.data = NULL;
    }
}