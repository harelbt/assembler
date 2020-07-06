#include "first pass.h"
#include "parser.h"
void first_pass(char* file) {
    FILE* filep = open_file(file, "r");
    line* sentence = allocate_arr_memory(1, "line");
    int line_number = 1;
    while (!read_line(filep, sentence)) {
        sentence->line_number = line_number;
        line_number++;
        if (strcmp(sentence->line, "\n")) {
            parse_line(sentence);

        }
    }
        fclose(filep);
        free(sentence);
}