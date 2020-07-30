#include "first pass.h"
#include "line analyzer.h"
#include "errors.h"
void first_pass(char* file, char* error_found) {
    /*opening file*/
    FILE *filep = open_file(file, "r");/*this custom function can handle malloc failure*/
    /*structs*/
    line sentence;
    line_marks_index indexes;
    line_marks_counter counters;
    /**/
    int line_number = 1;
    /**/
    counters.error_number = 0;
    while (!read_line(filep, &sentence)) {/*"read_line" returns 0 at EOF*/
        /*resets the variables for each line*/
        initialize_line_tools(&sentence, &counters, &indexes);
        /**/
        counters.line_number = line_number;
        line_number++;
        /**/
        /*enter is a line we want to count but not to read("read_line" can't skip '\n' because it needs to stop at the end of the line)*/
        if (strcmp(sentence.line, "\n") != 0) {
            analyze_sentence(&sentence, &indexes, &counters);/*parsing*/
            empty_or_comment_line_check(&sentence, indexes);
            /*continues if not a comment/empty line*/
                if (sentence.flags.is_comment == FALSE && sentence.flags.is_empty_line == FALSE) {
                    *error_found = errors_inspection(&sentence, indexes, &counters);
                    if(!*error_found){/*if no errors found*/
                    //build_sentence();
                    }
                }
        }
    }
    if (*error_found == TRUE) {
        print_errors_summary(file, counters.error_number);
    }
    free_first_pass(filep, &sentence);
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
    if (sentence->label.address != NULL){
        free(sentence->label.address);
        sentence->label.address = NULL;
    }
}