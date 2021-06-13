#include <stdlib.h>
#include <string.h>
#include "symbol table.h"
#include "in_out_tools.h"
/*
 * inserts a symbol to the symbol table
 */
symbol* insert_symbol(symbol* to_insert, symbol* symbol_table, char* is_first_symbol) {
    symbol *curr_pointer = symbol_table;
    symbol* new_symbol;
    /*in case this is the first symbol, fills the head*/
    if (*is_first_symbol == TRUE){
        to_insert->next = NULL;
        symbol_copy(symbol_table, to_insert);
        *is_first_symbol = FALSE;
        return symbol_table;
    }
    /*else prepares a new symbol to insert*/
    new_symbol = allocate_memory(1, SYMBOL);
    symbol_copy(new_symbol, to_insert);
    while (curr_pointer->next != NULL) {/*skips to the end of the table*/
        curr_pointer = curr_pointer->next;
    }
    /*inserts node*/
    curr_pointer->next = new_symbol;
    new_symbol->next = NULL;
    return symbol_table;
}
/*
 * updates a symbol address in struct line pointer.
 * if a label doesn't exist in the line does nothing
 */
void update_symbol_address(line* sentence, line_counters counters){
    if (sentence->flags.is_code == TRUE){/*line in an instruction line*/
        if (*sentence->label.name != '\0'){/*if a label exists*/
            /*calculation*/
            int distance_from_last_IC = counters.IC - counters.last_IC;
            sentence->label.address = counters.IC - (distance_from_last_IC);
        }
    } else{/*line in an order line*/
        if (*sentence->label.name != '\0'){
            if (strcmp(sentence->data_parts.order, "extern") != 0) {/*extern has alreadythe address of 1 (A)*/
                /*calculation*/
                int distance_from_last_DC = counters.DC - counters.last_DC;
                sentence->label.address = counters.DC - (distance_from_last_DC);
            } else{/*external label*/
                sentence->label.address = 1;
            }
        }
    }
}
/*
 * gets a string and a symbol table head.
 * if the string matches a symbol in the table returns the symbol*, else returns NULL
 */
void* get_symbol(symbol* symbol_table, char* symbol_name){
    symbol* symbol_pointer = symbol_table;
    while (symbol_pointer){
        if (!strcmp(symbol_name, symbol_pointer->name)){
            return symbol_pointer;
        }
        symbol_pointer = symbol_pointer->next;
    }
    return NULL;
}
/*
 * given a string that starts from a symbol name, returns the symbol name
 */
char* get_symbol_name(char* symbol_start){
    /*memory allocation to string to return*/
    char* symbol = allocate_memory(LABEL_MAX_LENGTH, SYMBOL);
    int i = 0;
    /*runs untill the end of the symbol and inserts the name to the string to return*/
    while (*symbol_start && *symbol_start != ' ' && *symbol_start != '\t' && *symbol_start != ',' && *symbol_start != '\n' && *symbol_start != '\r'){
        *(symbol + i) = *symbol_start;
        i++;
        symbol_start++;
    }
    *(symbol + i) = '\0';
    return symbol;
}
/*
 * frees a symbol table
 */
void free_symbol_table(symbol* symbol_table){
    while (symbol_table != NULL){/*until the table ends*/
        symbol* next_pointer = symbol_table->next;
        free(symbol_table);
        symbol_table = NULL;
        symbol_table = next_pointer;
    }
}
/*
 * copies source symbol node to a destination symbol node
 */
void symbol_copy(symbol* dest, symbol* source){
    strcpy(dest->name, source->name);
    dest->address = source->address;
    dest->external = source->external;
    dest->sentence_type = source->sentence_type;
    dest->next = source->next;
}
/*
 * inserts a "line_of_label_usage" node to the list that is in the "counters" variable.
 * the node contains the current line number of the current label usage (not declaration);
 * enters the node to the end of the list chronologically.
 */
void insert_symbol_usage_line(line_counters* counters) {
    line_of_label_usage *new_label_line;/*to insert*/
    line_of_label_usage* label_line_pointer = &counters->label_usage_line;/*helper pointer to the list*/
    if (counters->label_usage_line.line_number == 0){
        counters->label_usage_line.line_number = counters->line_number;
        counters->label_usage_line.next = NULL;
        return;
    }
    /*prepares the new node*/
    new_label_line = allocate_memory(ONE_UNIT, LINE_OF_LABEL);
    new_label_line->line_number = counters->line_number;
    new_label_line->next = NULL;
    /*skips to the end of the list*/
    while (label_line_pointer->next != NULL) {
        label_line_pointer = label_line_pointer->next;
    }
    label_line_pointer->next = new_label_line;/*inserts*/
}
/*
 * returns the line number of the the current symbol usage and frees the last one.
 * use only when a label usage shows up (not declaration), else values will be permanently lost.
 */
int get_symbol_usage_line(line_counters* counters){
    line_of_label_usage* label_line_pointer = &counters->label_usage_line;/*points to the start*/
    int line_number = label_line_pointer->line_number;/*to return(the current line)*/
    label_line_pointer = counters->label_usage_line.next;/*saves the address of the next node*/
    if (counters->label_usage_line.next != NULL) {
        counters->label_usage_line = *counters->label_usage_line.next;/*turns the value of the next node to the current value*/
    }
    free(label_line_pointer);/*freeing the allocated address of the node we made it's value to the current value*/
    return line_number;
}