#include <stdlib.h>
#include "data image.h"
void insert_data_node(data_image* head, data_image* to_insert){
    data_image* curr_pointer = head;
    if (head->is_head_filled == FALSE){
        head->word = to_insert->word;
        head->DC = to_insert->DC;
        head->is_head_filled = TRUE;
    }
    while (curr_pointer->next != NULL){
        curr_pointer = curr_pointer->next;
    }
        curr_pointer->next = to_insert;
}