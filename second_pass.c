#include <stdio.h>
#include <stdlib.h>
#include "in out tools.h"
#include "first pass.h"
#include "second_pass.h"
#include "errors.h"
#include "assembler data types.h"
#include "symbol table.h"
#include "stdlib.h"
#include "string.h"
#include "translator.h"
void second_pass(FILE * first_pass_file, symbol * symbol_table,FILE * input_file,
        line_counters* counters, char* error_found, char* file_name_without_type, char* is_entry, char* is_external){
    FILE* externals_file = create_ext_files(file_name_without_type);
    FILE* entries_file = create_ent_files(file_name_without_type);
    fseek(first_pass_file, 0, SEEK_SET);
    code_symbols(first_pass_file, symbol_table, externals_file, is_external);
    *is_entry = add_entries(input_file, symbol_table, entries_file, counters, error_found);

}
static FILE* create_ent_files(char* name_without_type){
    char ent_file_name[strlen(name_without_type) + TYPE_MAX_LENGTH + 1];
    strcpy(ent_file_name, name_without_type);
    strcat(ent_file_name, ".ent\0");
    return open_file(ent_file_name, "a+");
}
static FILE* create_ext_files(char* name_without_type){
    char ext_file_name[strlen(name_without_type) + TYPE_MAX_LENGTH + 1];
    strcpy(ext_file_name, name_without_type);
    strcat(ext_file_name, ".ext\0");
    return open_file(ext_file_name, "a+");
}
static void code_symbols(FILE* machine_code, symbol* symbol_table, FILE* externals_file, char* is_external) {
    char symbol_to_code[LABEL_MAX_LENGTH];
    char current_address_label[LABEL_MAX_LENGTH];
    int curr_char;
    typedef struct __attribute__((packed)) {
        unsigned int label_translated: 24;
    } address;
    address label;

    int distance;
    symbol *symbol_ptr;
    FILE *start_line = machine_code;
    while ((curr_char = fgetc(machine_code)) != EOF) {
        if (fgetc(machine_code) == '\n') {
            start_line = machine_code;
            fscanf(start_line, "%s", current_address_label);
        }
        if (curr_char == '?') {
            fprintf(machine_code, " ");//overriding the question mark
            if (fgetc(machine_code) == '&') {//need to put the distance of label
                fprintf(machine_code, " ");//overriding the & sign
                fscanf(machine_code, "%s", symbol_to_code);
                symbol_ptr = get_symbol(symbol_table, symbol_to_code);
                if (!strcmp(symbol_ptr->name, symbol_to_code)) {
                    distance = strtod(current_address_label, NULL) - symbol_ptr->address;
                    if (distance < 0) {
                        label.label_translated = abs(distance);
                        label.label_translated ^= TWOS_COMP_MASK;
                        (label.label_translated <<= 3u);
                        label.label_translated |= ABSOLUTE;//turn on the A bit
                    }
                    label.label_translated = (((unsigned) distance << 3u) | ABSOLUTE);//turn on the A bit
                    fprintf(machine_code, "%06x", label.label_translated);/*write in hexa the label address*/
                    if (symbol_ptr->extern_or_entry == EXTERN) {
                        print_entry_extern(externals_file, symbol_ptr);
                        *is_external = TRUE;
                    }

                } else {
                    fprintf(stderr, "The label %s does not exist in the file",
                            symbol_to_code); /*throw an error that there is no label*/
                }

            } else {//need to put in  address of the label
                fscanf(machine_code, "%s", symbol_to_code);
                symbol_ptr = get_symbol(symbol_table, symbol_to_code);
                if (!strcmp(symbol_ptr->name, symbol_to_code)) {
                    label.label_translated = symbol_ptr->address;
                    fprintf(machine_code, "%06x", label.label_translated);/*write in hexa the label address*/
                } else {
                    fprintf(stderr, "The label %s does not exist in the file",
                            symbol_to_code);/* throw an error that there is no label with such name*/
                }
            }
            while ((fgetc(machine_code)) != '\n') {//deleting all the writing that is not part of the translated label
                fprintf(machine_code, " ");
            }
        }
    }
}
static char add_entries(FILE* input_file, symbol* symbol_table, FILE* entries_file, line_counters* counters, char* error_found){
    char* line;
    char* entry;
    symbol* entry_symbol;
    int line_length = 0;
    char is_entry = FALSE;
    while (strcmp(line = get_line_dynamic(input_file, &line_length), "") != 0){
        entry = get_entry(line);
        if (strcmp(entry, "") != 0){
            entry_symbol = get_symbol(symbol_table, entry);
            if (entry_symbol == NULL){
                report_error(line, ENTRY_NOT_EXIST, counters);
                *error_found = TRUE;
            } else{
                is_entry = TRUE;
                print_entry_extern(entries_file, entry_symbol);
            }
        }
    }
    return is_entry;
}
static char* get_entry(char* line){
    char* i = line;
    char entry_check[ENTRY_ORDER_LENGTH];
    char* label = allocate_arr_memory(LABEL_MAX_LENGTH, CHAR);
    *(label + LABEL_MAX_LENGTH - 1) = '\0';
    *(entry_check + ENTRY_ORDER_LENGTH - 1) = '\0';
    while (*i){
        if (*i == 'e'){
            strncat(entry_check, i, ENTRY_ORDER_LENGTH - 1);
            if (!strcmp(entry_check, "entry")){
                i += ENTRY_ORDER_LENGTH - 1;
                while (*i == ' ' || *i == '\t'){
                    i++;
                }
                strncpy(label, i, LABEL_MAX_LENGTH - 1);
                return label;
            }
        }
        i++;
    }
    return "";
}
static void print_entry_extern(FILE* file, symbol* entry_extern){
    int address_length = get_address_length(entry_extern->address);
    int number_of_pre_zeros = ADDRESS_PRINT_LENGTH - address_length;
    fprintf(file, "%s ", entry_extern->name);
    if (number_of_pre_zeros > 0) {
        while (number_of_pre_zeros) {
            fprintf(file, "0");
            number_of_pre_zeros--;
        }
    }
    fprintf(file, "%d", entry_extern->address);
}
static int get_address_length(int address){
    int length = 0;
    while (address){
        address /= 10;
        length++;
    }
    return length;
}
/***** Ignore this *******/

/*while(*input_file!=EOF){
    resetString(string);
    i = 0;
    while(*input_file!= '\t' || *input_file!='\n' ||*input_file!=' '|| *input_file!= ':'){
        string[i] = *input_file;
        input_file++; i++;
    }
    input_file++;*/
    /* if the string is in the map symbol */
    /*get its details from the map symbol*/
        /*if the label does not contains '&'*/
             /*while line does not contain question mark in file first_pass_file so continue (find the line with the question mark)*/
             /*while line in file first_pass_file contains question mark do:
            *** this doesnt need to be a loop because there is only one line missing****/

            /*if the label is extern so put in ARE the variable E*/
            /*if the label is relocate able so put in ARE the variable R*/
            /*if the label is constant put in ARE the variable A*/
            /*insert the label address to the first pass file*/
       /*else*/
            /*calculate the distance of the line to the address of the label*/
            /*if the distance is positive so put into labelAddress1 the distance*/
            /*if its negative make the number negative by using the complement 2 method(make a function of complement 2)*/
            /*put the ARE bits in its right position*/

