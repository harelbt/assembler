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
    if (error_list->error == NULL) {
        create_error(error_list, message, line);
        return;
    }
    create_error(new_error, message, line);
    curr_pointer = error_list;
    prev_pointer = NULL;

    while (curr_pointer->line < line && curr_pointer->next != NULL) {
        prev_pointer = curr_pointer;
        curr_pointer = curr_pointer->next;
    }
    if (curr_pointer->line < line){
        curr_pointer->next = new_error;
    } else {
        new_error->next = curr_pointer;
        prev_pointer->next = new_error;
    }
}
void output_errors(error* error_list){
    error* curr_pointer = error_list;
    error* prev_pointer = NULL;
    while (curr_pointer->next != NULL) {
        printf("ERROR: %s. in line %d", curr_pointer->error, curr_pointer->line);
        prev_pointer = curr_pointer;
        curr_pointer = curr_pointer->next;
        free(prev_pointer);
    }
    printf("ERROR: %s. in line %d", curr_pointer->error, curr_pointer->line);
    free(curr_pointer);
}
static void create_error(error* new_list, char* message, int line){
    new_list->error = message;
    new_list->line = line;
    new_list->next = NULL;
};