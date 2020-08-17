#include <stdio.h>
#include <stdlib.h>
#include "in out tools.h"
#include "second_pass.h"
#include "errors.h"
#include "symbol table.h"
#include <string.h>
#include "translator.h"
#include <ctype.h>
void second_pass(FILE * machine_code, FILE* temp_machine_code, symbol * symbol_table, FILE * input_file, line_counters* counters, char* error_found,
                char* file_name_without_type){
    FILE* externals_file = create_ext_files(file_name_without_type);
    FILE* entries_file = create_ent_files(file_name_without_type);
    char is_entry = FALSE;
    char is_external = FALSE;
    code_symbols(machine_code, temp_machine_code, symbol_table, counters, externals_file, &is_external, error_found);
    if (*error_found == FALSE) {
        is_entry = add_entries(input_file, symbol_table, entries_file, counters, error_found);
    }
    close_files(externals_file, entries_file, machine_code);
    free_symbol_table(symbol_table);
    remove_unnecessary_files(file_name_without_type, error_found, is_external, is_entry);
}
/**/
static void code_symbols(FILE* machine_code, FILE* temp_machine_code, symbol* symbol_table, line_counters* counters, FILE* externals_file, char* is_external, char* error_found) {
    typedef struct __attribute__((packed)) {
        unsigned int label_address_binary: 24;
    } address;
    address label_address;
    symbol *symbol_to_code;
    char* curr_line;
    char curr_address[NUMBER_ALLOWED_LENGTH];
    int curr_address_iterator = 0;
    int curr_char;
    int line_length;
    int distance;
    char *i;
    fseek(temp_machine_code, START, SEEK_SET);
    while ((curr_char = fgetc(temp_machine_code)) != EOF) {
        if (curr_char >= '0' && curr_char <= '9'){
            *(curr_address + curr_address_iterator) = (char) curr_char;
            curr_address_iterator++;
        }
        if (curr_char == ' ' || curr_char == '\t' || curr_char == '\n' || curr_char == '\r'){
            *(curr_address + curr_address_iterator) = '\0';
            curr_address_iterator = 0;
        }
        if (curr_char == '?') {
            label_address.label_address_binary = 0;
            curr_line = get_line_dynamic(temp_machine_code, &line_length);
            i = curr_line;
                    if (*(i) == '&') {
                        i++;
                        symbol_to_code = get_symbol(symbol_table, get_symbol_name(i));
                    }
                    if (symbol_to_code == NULL) {
                        report_error(i, LABEL_DOESNT_EXIST, counters);
                        *error_found = TRUE;
                    } else {
                        if (*(i-1) == '&') {
                            distance = symbol_to_code->address - ((int) strtod(curr_address, NULL) - ONE_WORD);
                            if (distance < 0) {
                                label_address.label_address_binary = abs(distance);
                                label_address.label_address_binary ^= ONES_COMP_MASK;
                                label_address.label_address_binary += COMPLEMENT_TO_TWO;
                                label_address.label_address_binary <<= 3u;
                                label_address.label_address_binary |= ABSOLUTE;//turn on the A bit
                            } else {
                                label_address.label_address_binary = (((unsigned) distance << 3u) |
                                                                      ABSOLUTE);//turn on the A bit
                            }
                            fprintf(machine_code, "%06x\n",
                                    (signed int) label_address.label_address_binary);/*write in hexa the labels address */
                        } else {
                            symbol_to_code = get_symbol(symbol_table, get_symbol_name(i));
                            if (symbol_to_code == NULL) {
                                report_error(i, LABEL_DOESNT_EXIST, counters);
                                *error_found = TRUE;
                            } else {
                                if (symbol_to_code->external == EXTERN) {
                                    print_extern(externals_file, symbol_to_code, curr_address);
                                    *is_external = TRUE;
                                    label_address.label_address_binary |= EXTERNAL;
                                }
                                if (symbol_to_code->external != EXTERN) {
                                    label_address.label_address_binary = symbol_to_code->address;
                                    if (symbol_to_code->sentence_type == DATA){
                                        label_address.label_address_binary += counters->IC;
                                    }
                                    label_address.label_address_binary <<= 3U;
                                    label_address.label_address_binary |= RELOCATABLE;
                                }
                            }
                            fprintf(machine_code, "%06x\n",
                                    (signed int) label_address.label_address_binary);/*write in hexa the labels address */
                        }
                    }
        } else{
            fputc(curr_char, machine_code);
        }
    }
}
/*char symbol_to_code[LABEL_MAX_LENGTH];
char current_address_label[LABEL_MAX_LENGTH];
int curr_char;
typedef struct __attribute__((packed)) {
    unsigned int label_address_binary: 24;
} address;
address label_address;
int distance;
symbol *symbol_ptr;
FILE *start_line;
fseek(machine_code, START, SEEK_SET);
while ((curr_char = fgetc(machine_code)) != EOF) {
    if (curr_char == '\n') {// starting of a new line
        start_line = machine_code;
        fscanf(start_line, "%s", current_address_label);// getting the IC of the label
    }
    if (curr_char == '?') {//this condition makes sure that there is a symbol need to be translated to machine code
      //fprintf(machine_code, " ");//overriding the question mark
      fscanf(machine_code, symbol_to_code);
        if (*symbol_to_code == '&') {//need to put the distance of label_address
            //fprintf(machine_code, " ");//overriding the & sign
            //fscanf(machine_code, "%s", symbol_to_code);//getting the name of the label
            symbol_ptr = get_symbol(symbol_table, symbol_to_code + 1);//checking that the name of the label is in the symbol table
            if (symbol_ptr == NULL){
                report_error(symbol_to_code,LABEL_DOESNT_EXIST,counters);
                *error_found = TRUE;
            } else {
                if (!strcmp(symbol_ptr->name, symbol_to_code)) {
                    distance = (int) strtod(current_address_label, NULL) - symbol_ptr->address;
                    if (distance < 0) {
                        label_address.label_address_binary = abs(distance);
                        label_address.label_address_binary ^= TWOS_COMP_MASK;
                        label_address.label_address_binary <<= 3u;
                        label_address.label_address_binary |= ABSOLUTE;//turn on the A bit

                    }
                    label_address.label_address_binary = (((unsigned) distance << 3u) |
                                                          ABSOLUTE);//turn on the A bit
                    fprintf(machine_code, "%06x",
                            (signed int) label_address.label_address_binary);/*write in hexa the labels address */
                        /*if (symbol_ptr->external == EXTERN) {
                            print_extern(externals_file, symbol_ptr);
                            is_external = TRUE;
                        }
                    }
                }

            }
            else {//need to put in  address of the label_address
                fscanf(machine_code, "%s", symbol_to_code);
                symbol_ptr = get_symbol(symbol_table, symbol_to_code);
                if (symbol_ptr == NULL){
                    report_error(symbol_to_code,LABEL_DOESNT_EXIST,counters);
                    *error_found = TRUE;
                } else {
                    if (!strcmp(symbol_ptr->name, symbol_to_code)) {
                        label_address.label_address_binary = symbol_ptr->address;
                        label_address.label_address_binary <<= 3u;
                        if (symbol_ptr->external == EXTERN)
                            label_address.label_address_binary |= EXTERNAL;//turn on the E bit
                        else
                            label_address.label_address_binary |= RELOCATABLE;//turn on the R bit

                        fprintf(machine_code, "%06x",
                                (signed int) label_address.label_address_binary);/*write in hexa the label_address address*/
                    /*}
                }
            }*/
           /* while ((fgetc(machine_code)) != '\n') {//deleting all the writing that is not part of the translated label_address
                fprintf(machine_code, " ");
            }*/
        //}
    /*}
}*/
static char add_entries(FILE* input_file, symbol* symbol_table, FILE* entries_file, line_counters* counters, char* error_found){
    char* line;
    char* entry;
    symbol* entry_symbol;
    int line_length = 0;
    char is_entry = FALSE;
    fseek(input_file, START, SEEK_SET);
    counters->line_number = 0;
    while (strcmp(line = get_line_dynamic(input_file, &line_length), "") != 0){
        counters->line_number++;
        entry = get_entry(line);
        if (strcmp(entry, "") != 0){
            entry_symbol = get_symbol(symbol_table, entry);
            if (entry_symbol == NULL){
                report_error(line, ENTRY_NOT_EXIST, counters);
                *error_found = TRUE;
            } else{
                is_entry = TRUE;
                print_entry(entries_file, entry_symbol);
            }
        }
        free(line);
        line = NULL;
        if (strcmp(entry, "") != 0) {
            free(entry);
            entry = NULL;
        }
    }

    return is_entry;
}
static char* get_entry(char* line){
    char* i = line;
    char entry_check[ENTRY_ORDER_LENGTH] = {'\0'};
    char* label = allocate_memory(LABEL_MAX_LENGTH, CHAR);
    int index = 0;
    *(label) = '\0';
    //*(entry_check) = '\0';
    while (*i && *i != '\"' && *i != ','){
        if (*i == 'e'){
            strncpy(entry_check, i, ENTRY_ORDER_LENGTH - 1);
            if (!strcmp(entry_check, "entry")){
                i += ENTRY_ORDER_LENGTH - 1;
                while (*i == ' ' || *i == '\t'){
                    i++;
                }
                strncpy(label, i, LABEL_MAX_LENGTH - 1);
                while (*(label + index)){
                    if (*(label + index) == '\t' || *(label + index) == ' '){
                        *(label + index) = '\0';
                    }
                    index++;
                }
                return label;
            }
        }
        i++;
    }
    free(label);
    return "";
}
static char* get_symbol_name(char* symbol_start){
    char* symbol = allocate_memory(LABEL_MAX_LENGTH, SYMBOL);
    int i = 0;
    while (*symbol_start && *symbol_start != ' ' && *symbol_start != '\t' && *symbol_start != ',' && *symbol_start != '\n' && *symbol_start != '\r'){
        *(symbol + i) = *symbol_start;
        i++;
        symbol_start++;
    }
    *(symbol + i) = '\0';
    return symbol;
}
static char* get_current_address(char* line){
    char* i = line;
    char* address = allocate_memory(NUMBER_ALLOWED_LENGTH, CHAR);
    int k = 0;
    char is_started = FALSE;
    while (*i && *i != ' '){
        if (is_started == FALSE && *i != '0'){
            is_started = TRUE;
        }
        if (is_started == TRUE){
            *(address + k) = *i;
        }
        i++;
    }
    return address;
}
static void close_files(FILE *externals_file, FILE *entries_file, FILE *machine_code) {
    fclose(externals_file);
    fclose(entries_file);
    fclose(machine_code);
}