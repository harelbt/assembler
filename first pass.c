#include "first pass.h"
#include "line analyzer.h"
#include "errors.h"
void first_pass(char* file, short int* error_found) {
    FILE *filep = open_file(file, "r");
    line sentence;
    line_marks_index indexes;
    line_marks_counter counters;
    counters.error_number = 0;
    int line_number = 1;
    while (!read_line(filep, &sentence)) {
        initialize_line_tools(&sentence, &counters, &indexes);
        counters.line_number = line_number;
        line_number++;
        if (strcmp(sentence.line, "\n") != 0) {
            analyze_sentence(&sentence, &indexes, &counters);
            empty_or_comment_line_check(&sentence, indexes);
                if (sentence.flags.is_comment == FALSE && sentence.flags.is_empty_line == FALSE) {
                    *error_found = errors_inspection(&sentence, indexes, &counters);
                    if(!*error_found){
                    //build_sentence();
                    }
                }
        }
    }
    if (*error_found == 1) {
        print_errors_summary(file, counters.error_number);
    }
    free_first_pass(filep, &sentence);
}
static void print_errors_summary(char* file_name, int errors_count){
    printf("\nFILE: %s\n", file_name);
    printf("%d ERRORS WAS FOUND\n", errors_count);
    puts("NO OUTPUT FILES WERE GENERATED"
         "\n*************************************************************************************");
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
    if (sentence->code_parts.first_operand != NULL) {
        free(sentence->code_parts.first_operand);
        sentence->code_parts.first_operand = NULL;
    }
    if (sentence->code_parts.second_operand != NULL) {
        free(sentence->code_parts.second_operand);
        sentence->code_parts.second_operand = NULL;
    }
    if (sentence->data_parts.data != NULL){
        free(sentence->data_parts.data);
        sentence->data_parts.data = NULL;
    }
    if (sentence->label.address != NULL){
        free(sentence->label.address);
        sentence->label.address = NULL;
    }
}