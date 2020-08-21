#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "in_out_tools.h"
#include "second_pass.h"
#include "errors.h"
#include "symbol table.h"
#include "translator.h"
/*
 * completes the labels code and creates the .ext .ent files and writes to them.
 * if no entries were found removes .ent
 * if no externals were found removes .ext
 *if a label is used but not declared produces an error
 */
void second_pass(FILE * machine_code, FILE* temp_machine_code, symbol * symbol_table, FILE * input_file, line_counters* counters, char* error_found,
                char* file_name_without_type){
    /*the .ext file*/
    FILE* externals_file = create_ext_files(file_name_without_type);
    /*the .ent file*/
    FILE* entries_file = create_ent_files(file_name_without_type);
    char is_entry = FALSE;/*flag to know if entries were found*/
    char is_external = FALSE;/*flag to know if externals were found*/
    /*completes the labels code and copies the temp file to the .ob file*/
    code_symbols(machine_code, temp_machine_code, symbol_table, counters, externals_file, &is_external, error_found);
    /*writes entries to .ent if no errors found*/
    if (*error_found == FALSE) {
        is_entry = add_entries(input_file, symbol_table, entries_file, counters, error_found);
    }
    close_files(externals_file, entries_file, machine_code);
    remove_unnecessary_files(file_name_without_type, error_found, is_external, is_entry);
}
/*
 * completes the labels code and copies the temp file to the .ob file and sets the is_external flag
 */
void code_symbols(FILE* machine_code, FILE* temp_machine_code, symbol* symbol_table, line_counters* counters, FILE* externals_file, char* is_external, char* error_found) {
    label_address label_address;/*struct the holds the address in 24 bits*/
    char *curr_line;/*hold a line from the assembly code when a label shows up*/
    /*holds the address that is written in the line of the current line in the machine code*/
    char curr_address[NUMBER_ALLOWED_LENGTH];
    int curr_address_iterator = 0;/*used as index to insert chars to "curr_address" array*/
    int curr_char;
    int line_length;/*required as an argument for "get_line_dynamic" function that is being used next*/
    char *i;/*pointer to point to the start of the label*/
    counters->line_number = 1;/*to track the line number*/
    fseek(temp_machine_code, START, SEEK_SET);
    while ((curr_char = fgetc(temp_machine_code)) != EOF) {/*until file ends*/
        if (curr_char >= '0' && curr_char <= '9') {/*inserts address from the temp machine code to the "curr_address" array*/
            *(curr_address + curr_address_iterator) = (char) curr_char;
            curr_address_iterator++;
        }
        /*address ended, resets "curr_address_iterator" index and puts '\0'*/
        if (curr_char == ' ' || curr_char == '\t' || curr_char == '\n' || curr_char == '\r') {
            *(curr_address + curr_address_iterator) = '\0';
            curr_address_iterator = 0;
        }
        if (curr_char == '?') {/*a label was found*/
            label_address.label_address_binary = 0;
            /*gets the line from the temp machine code*/
            curr_line = get_line_dynamic(temp_machine_code, &line_length);
            i = curr_line;
            if (*(i) == '&') {/*a "jump to label"*/
                i++;
                code_jump(machine_code, symbol_table, i, counters, &label_address, curr_address, error_found);
            }
            else
                code_label_address(machine_code, externals_file, symbol_table, i, counters, &label_address, curr_address, error_found, is_external);
            if (strcmp(curr_line, "") != 0)/*if get_line_dynamic returned "" it not allocated dynamically*/
                free(curr_line);

        }
        else {
            fputc(curr_char, machine_code);/*copies temp to .ob*/
        }
    }
}
/*
 * looks for the .entry orders end writes to .ent
 * if entry is used but label isn't declared produces an error
 * if entries found returns TRUE, else FALSE
 */
char add_entries(FILE* input_file, symbol* symbol_table, FILE* entries_file, line_counters* counters, char* error_found){
    char* line;/*the current line in the input file*/
    char* entry;/*holds the name of the entry*/
    symbol* entry_symbol;
    int line_length = 0;/*of "line"*/
    char is_entry = FALSE;/*flag to return*/
    fseek(input_file, START, SEEK_SET);
    counters->line_number = 0;/*to report errors according to th line number*/
    while (strcmp(line = get_line_dynamic(input_file, &line_length), "") != 0){
        counters->line_number++;
        entry = get_entry(line);/*gets label*/
        if (strcmp(entry, "") != 0){/*if there is an entry*/
            entry_symbol = get_symbol(symbol_table, entry);
            if (entry_symbol == NULL){/*symbol isn't declared, reports an error*/
                report_error(line, ENTRY_NOT_EXIST, counters);
                *error_found = TRUE;
            } else{/*prints entry to .ent*/
                is_entry = TRUE;
                print_entry(entries_file, entry_symbol, counters->IC);
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
/*
 * gets a line and if .entry exists returns the label with no white chars before or after
 */
char* get_entry(char* line){
    char* i = line;/*pointer to to check the line*/
    /*holds chars from the input to check if .entry exists*/
    char entry_check[ENTRY_ORDER_LENGTH] = {'\0'};
    char* label = allocate_memory(LABEL_MAX_LENGTH, CHAR);/*to return*/
    int index = 0;/*to fill "label"*/
    *(label) = '\0';
    while (*i && *i != '\"' && *i != ',' && *i != ';'){/*until the end of the line or the an enexpected char*/
        if (*i == 'e'){/*suspects for .entry order*/
            strncpy(entry_check, i, ENTRY_ORDER_LENGTH - 1);/*gets the rest of the word in the length of entry*/
            if (!strcmp(entry_check, "entry")){/*if it's really an entry order gets the label*/
                i += ENTRY_ORDER_LENGTH - 1;/*moves i to the end of "emtry"*/
                while (*i == ' ' || *i == '\t'){/*skips to the label*/
                    i++;
                }
                strncpy(label, i, LABEL_MAX_LENGTH - 1);/*gets the label*/
                while (*(label + index)){/*puts a '\0' when a white char shows up to seal the label*/
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
    return "";/*.entry doesn't exist*/
}
/*closes .ent .ext .ob*/
void close_files(FILE *externals_file, FILE *entries_file, FILE *machine_code) {
    fclose(externals_file);
    fclose(entries_file);
    fclose(machine_code);
}