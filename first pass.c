#include "first pass.h"
#include "line analyzer.h"
#include "helpfunctions.h"
#include "errors.h"
void first_pass(char* file, short int* error_found) {
    FILE *filep = open_file(file, "r");
    line sentence;
    line_marks_index indexes;
    line_marks_counter counters;

    int line_number = 1;
    while (!read_line(filep, &sentence)) {
        initialize_line_tools(&sentence, &counters, &indexes);
        counters.line_number = line_number;
        line_number++;
        if (strcmp(sentence.line, "\n") != 0) {
            analyze_sentence(&sentence, &indexes, &counters);
            empty_line_check(&sentence, indexes);
            if (sentence.flags.is_empty_line == FALSE) {
                comment_check(&sentence, indexes);
                if (sentence.flags.is_comment == FALSE) {
                    *error_found = errors_inspection(sentence, indexes, counters);
                    if(!*error_found){
                    //build_sentence();
                    }
                }
            }
        }
    }
    fclose(filep);
    free(filep);
    free(sentence.line);
    filep = NULL;
    sentence.line = NULL;
    sentence.code_parts.second_operand = NULL;
}