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
#include "first pass.h"
#include "assembler.h"
#include "translator.h"
int second_pass(FILE * first_pass_file, symbol * symbol_table,FILE * input_file,symbol** symbol_addresses_table,
        line_counters* counters, char* error_found, char* file_name_without_type, char* is_entry, char* is_external){
    FILE** externals_file = NULL;
    FILE** entries_file = NULL;
    create_ext_ent_files(file_name_without_type, externals_file, entries_file);
    fseek(first_pass_file, 0, SEEK_SET);
    code_symbols(first_pass_file, symbol_table, *externals_file, is_external);
    *is_entry = add_entries(input_file, symbol_table, *entries_file, symbol_addresses_table, counters, error_found);

}
static void create_ext_ent_files(char* name_without_type, FILE** externals_file, FILE** entries_file){
    char ext_file_name[strlen(name_without_type) + TYPE_MAX_LENGTH];
    char ent_file_name[strlen(name_without_type) + TYPE_MAX_LENGTH];
    strcpy(ext_file_name, name_without_type);
    strcpy(ent_file_name, name_without_type);
    strcat(ext_file_name, ".ext");
    strcat(ent_file_name, ".ent");
    *externals_file = open_file(ext_file_name, "a+");
    *entries_file = open_file(ent_file_name, "a+");
}
static void code_symbols(FILE* machine_code, symbol* symbol_table, FILE* externals_file, char* is_external){
    char symbol_to_code[LABEL_MAX_LENGTH];
    char current_address_label[LABEL_MAX_LENGTH];
    int curr_char;
    typedef struct __attribute__((packed)){
        unsigned int label_address: 24;
    }address;
    address label;

    int distance;
    symbol* symbol_ptr;
    FILE * start_line = machine_code;
    while ((curr_char = fgetc(machine_code)) != EOF){
        if(fgetc(machine_code)=='\n'){
            start_line = machine_code;
            fscanf(start_line,"%s",current_address_label);
        }
        if(curr_char == '?'){
            if(fgetc(machine_code) == '&'){//need to put the distance of label
                fscanf(machine_code,"%s",symbol_to_code);
                symbol_ptr = get_symbol(symbol_table, symbol_to_code, &symbol_table);
                if(!strcmp(symbol_ptr->name, symbol_to_code)){
                    distance = strtod(current_address_label, NULL) - symbol_ptr->address;
                    if(distance<0){
                        label.label_address = abs(distance);
                        label.label_address ^= TWOS_COMP_MASK;
                        (label.label_address <<= 3u);
                        label.label_address |= ABSOLUTE;//turn on the A bit

                    }
                    else
                        label.label_address = (((unsigned )distance << 3u)|ABSOLUTE);
                    writeHexa(label.label_address,machine_code);/*write in hexa the address*/
                    if (symbol_ptr->extern_or_entry == EXTERN){
                        print_entry_extern(externals_file, symbol_ptr);
                    }

                }
                else{
                    fprintf(stderr,"The label %s does not exist in the file",symbol_to_code); /*throw an error that there is no label*/
                }

                }
            else{//need to put in  address of the label
                fscanf(machine_code,"%s",symbol_to_code);
                symbol_ptr = get_symbol(symbol_table, symbol_to_code, &symbol_table);
                if(strcmp(symbol_ptr->name, symbol_to_code)){
                    label.label_address = symbol_ptr->address;
                    writeHexa(label.label_address,machine_code);/*write in hexa the label address*/
                }
                else{
                    fprintf(stderr,"The label %s does not exist in the file",symbol_to_code);/* throw an error that there is no label with such name*/
                }
            }

        }
    }
}
void writeHexa(unsigned int label,FILE * file){

}
static void print_external(){

}
static char add_entries(FILE* input_file, symbol* symbol_table, FILE* entries_file, symbol** symbol_addresses_table, line_counters* counters, char* error_found){
    char* line;
    char* entry;
    symbol* entry_symbol;
    int line_length = 0;
    char is_entry = FALSE;
    while (strcmp(line = get_line_dynamic(input_file, &line_length), "") != 0){
        entry = get_entry(line);
        if (strcmp(entry, "") != 0){
            entry_symbol = get_symbol(symbol_table, entry, symbol_addresses_table);
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
static void prepare_output_files(){}

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



void resetString(char str[STRING_LEN]){
    int i;
    for (i = 0; i <STRING_LEN; ++i)
        str[i]='\0';
}
