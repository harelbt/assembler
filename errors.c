#include <stdlib.h>
#include "errors.h"
#include "helpfunctions.h"
void report_error(line* sentence, short int error_code, error* error_list){
    char* message;
    switch (error_code) {
        case UNEXPECTED_SEMICOLON:{
            message = "unexpected semicolon.";
            break;
        }
        case UNEXPECTED_HASHMARK:{
            message = "unexpected hash mark.";
            break;
        }
    }
    insert_error(error_list,message,sentence->line_number, sentence->char_number);
}
static void insert_error(error* error_list, char* message, int line, int char_number) {
    error *new_error = allocate_arr_memory(1, "error");
    if (error_list->error == NULL) {
        create_error(error_list, message, line, char_number);
        return;
    }
    create_error(new_error, message, line, char_number);
    insert_node(error_list, new_error);
}
void output_errors(error* error_list){
    error* curr_pointer = error_list;
    error* prev_pointer = NULL;
    while (curr_pointer->next != NULL) {
        printf("ERROR: %s in line %d:%d\n", curr_pointer->error, curr_pointer->line, curr_pointer->char_number+1);
        prev_pointer = curr_pointer;
        curr_pointer = curr_pointer->next;
        free(prev_pointer);
    }
    printf("ERROR: %s in line %d:%d", curr_pointer->error, curr_pointer->line, curr_pointer->char_number+1);
    free(curr_pointer);
}
static void create_error(error* new_list, char* message, int line, int char_number){
    new_list->error = message;
    new_list->line = line;
    new_list->char_number = char_number;
    new_list->next = NULL;
}
void insert_node(error* error_list, error* new_error){
    error* curr_pointer = error_list;
    while (curr_pointer->next != NULL) {
        curr_pointer = curr_pointer->next;
    }
    curr_pointer->next = new_error;
}