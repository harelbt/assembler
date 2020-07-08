#include "first pass.h"
#include "line analyzer.h"
#include "helpfunctions.h"
#include "errors.h"
void first_pass(char* file) {
    FILE *filep = open_file(file, "r");
    line sentence;
    line_marks_index indexes;
    line_marks_counter counters;
    initialize_line_tools(&sentence, &counters, &indexes);
    int line_number = 1;
    while (!read_line(filep, &sentence)) {
        sentence.line_number = line_number;
        line_number++;
        if (strcmp(sentence.line, "\n") != 0) {
            analyze_sentence(&sentence, &indexes, &counters);
            empty_line_check(&sentence, indexes);
            if (sentence.flags.is_empty_line == FALSE) {
                comment_check(&sentence, indexes);
                if (sentence.flags.is_comment == FALSE) {
                    //if(!errors_inspection()){
                    //build_sentence();
                    //}
                }
            }

        }
    }
    fclose(filep);
    free(filep);
    filep = NULL;
}