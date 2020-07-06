#include "first pass.h"
#include "parser.h"
void first_pass(char* file){
    FILE* filep = open_file(file,"r");
    line* sentence = allocate_arr_memory(1,"line");
    error error_list;
    error_list.error = NULL;
    int line_number = 1;
    while (!read_line(filep, sentence)) {
        sentence->line_number = line_number;
        line_number++;
        if (strcmp(sentence->line, "\n")) {
            parse_line(sentence, &error_list);
            if (error_list.error != NULL){}
        }
    }
    fclose(filep);
    free(sentence);
}

