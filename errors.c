#include <stdlib.h>
#include "errors.h"
#include "helpfunctions.h"
error* report_error(line* sentence, short int error_code, error* error_list){
    char* message;
    switch (error_code) {
        case UNEXPECTED_SEMICOLON:{
            message = "unexpected semicolon.";
        }

    }
    insert_error(error_list,message,sentence->line_number);
    return error_list;
}
void insert_error(error* error_list, char* message, int line) {
    error *new_error = NULL;
    error *curr_pointer;
    error *prev_pointer;
    if (error_list == NULL) {
        create_error(error_list, message, line);
        return;
    }
    create_error(new_error, message, line);
    curr_pointer = error_list;
    prev_pointer = NULL;

    while (error_list->line < line && curr_pointer->next != NULL) {
        prev_pointer = curr_pointer;
        curr_pointer = curr_pointer->next;
    }

}
void output_errors(error* error_list){

}
static void create_error(error* new_list, char* message, int line){
    new_list = allocate_arr_memory(1, "error");
    new_list->error = message;
    new_list->line = line;
    new_list->next = NULL;
};