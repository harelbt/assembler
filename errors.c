#include <stdlib.h>
#include "errors.h"
#include "helpfunctions.h"
#include "linked list.h"
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
static void insert_error(error* error_list, char* message, int line) {
    error *new_error = allocate_arr_memory(1, "error");
    if (error_list->error == NULL) {
        create_error(error_list, message, line);
        return;
    }
    create_error(new_error, message, line);
    insert_node("error", error_list, new_error);

}
void output_errors(error* error_list){
    error* curr_pointer = error_list;
    error* prev_pointer = NULL;
    while (curr_pointer->next != NULL) {
        printf("ERROR: %s in line %d\n", curr_pointer->error, curr_pointer->line);
        prev_pointer = curr_pointer;
        curr_pointer = curr_pointer->next;
        free(prev_pointer);
    }
    printf("ERROR: %s in line %d", curr_pointer->error, curr_pointer->line);
    free(curr_pointer);
}
static void create_error(error* new_list, char* message, int line){
    new_list->error = message;
    new_list->line = line;
    new_list->next = NULL;
};