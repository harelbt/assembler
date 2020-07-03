#include "linked list.h"
#include <stdarg.h>
#include <string.h>
#include "errors.h"
void insert_node(char* type,...){
    va_list arg_pointer;
    va_start(arg_pointer, type);
    if (!strcmp(type, "error")){
        error* curr_pointer = va_arg(arg_pointer, error*);
        error* new_error = va_arg(arg_pointer, error*);
        while (curr_pointer->next != NULL) {
            curr_pointer = curr_pointer->next;
        }
        curr_pointer->next = new_error;
    }
    va_end(arg_pointer);
}
