/*
 * Creators:
 * Gavriel Aggasi ID:207902297
 * Harel Bentabou ID:206911224
 * ---
 * GENERAL FUNCTIONING:
 * This program translates assembly code to machine code of an imaginary cpu.
 * input is through command line arguments. can take unlimited number of .as files.
 * generates .ob file(machine code) if no errors found. else prints errors to stdout. and no out files will be generated
 * if entries are defined in the assembly and no errors found, generates .ent file with the name and the address of the entry.
 * if externals are defined in the assembly and no errors found, generates .ext file with the name and the address of
 * the the address of the use, a line for every external use.
 * MECHANISM:
 * 1)FIRST PASS: checks errors, translates everything but the labels and creates a symbol table.
 * in the first pass label are printed to the output file with '?' before the label. if it's a label with '&', prints
 * "?&" before the label.
 * 2)SECOND PASS: codes the labels and creates the .ext .ent files and writes to them.
 * 3)in the end removes unnecessary files, .ob if errors found, .ext . ent if errors were found or no entries or no
 * external found
 *ASSUMPTIONS:
 * code line refers to instruction lne, order/data line refers to lines with .data/.string/.extern/.entry orders.
 * an order is .data/.string/.extern/.entry.
 * "line" and "sentence" refers to an assembly line.
 * SETTINGS:
 * flags are declared with char to save space.
 * structs that are not being taken by their address are compressed to their real required size using __attribute__((packed)).
 * "line_counters" struct lives through all the program. "line_indexes" and "ine" structs live through the first pass.
 * all the data types created in this program are defined in "assembler data types.h"
 * DATA STRUCTURES:
 * the data image and the symbol table are built as linked lists. because it fits with the use of these tables and
 * allows max usage of the memory.
 * data image is implemented with fifo mechanism to print the data chronologically after the instructions translation
 * IMPORTANT: "line_of_label_usage" type, is a linked list with a fifo mechanism that is declared in the line_counters struct.
 * it allows the second pass to print a line number in case of an error of a usage of an undeclared label.
 * (since the symbol coding is done using only by the temporary machine code and the symbol table).
 * when a label usage (not declaration) shows up in the first pass, the line number enters to the end of the list.
 * when a label usage shows up in the second pass, it's the same label usage from the first pass. so the first node
 * is the first label usage's line number. so the second pass get's the first node's value, releases the memory,
 * list's head is now the next node.
 *
 * */
#include <stdio.h>
#include <stdlib.h>/*for EXIT_FAILURE macro*/
#include <string.h>
#include "first pass.h"
#include "second_pass.h"
#include "symbol table.h"
int main (int argc, char* argv[]) {
    /*errors_flag*/
    char error_found = FALSE;
    /*files*/
    FILE *temp_machine_code;/*temporary file for first pass translation. without the first line of the words count(ic-100 dc-100)*/
    FILE *machine_code;/*the .ob file*/
    FILE *source;/*input file*/
    /*name without type for opening and removing different types of files(.ob, .ent, .ext)*/
    char *file_name_without_type;
    /**/
    int i = 1;
    /*checks if files wasn't supplied*/
    if (argc == 1) {
        /*prints this error to stderr with exit code 1*/
        stop(EXIT_FAILURE, "no assembly code was supplied.\nassembly codes can be given as command line arguments\n");
    }
    /*assembles each supplied file, all the program executes through this loop*/
    while (i < argc) {
        /*a struct that contains all the necessary counters for the program, including IC, DC*/
        line_counters counters;
        /*symbol_table_head*/
        symbol *symbol_table = allocate_memory(ONE_UNIT, SYMBOL);/*this custom function can handle malloc failure*/
        /**/
        *(symbol_table->name) = '\0';
        symbol_table->next = NULL;
        counters.label_usage_line.line_number = 0;
        /*OPENING FILES*/
        source = fopen(*(argv + i), "r");
        if (source != NULL) {
            file_name_without_type = get_file_name_without_type(*(argv + i));
            if (strcmp(file_name_without_type, "") != 0) {
                machine_code = open_machine_code(file_name_without_type, "w+");
                temp_machine_code = open_file("temp.TXT", "w+");/*this custom function can handle fopen failure*/
                /*ASSEMBLING*/
                /*analyzes the input, finds errors and produces the machine code (.ob) without the labels coding*/
                first_pass(source, temp_machine_code, symbol_table, &counters, &error_found);
                if (error_found == FALSE) {
                    /*prints the words count (instructions and data count) to the first line of the machine code*/
                    print_words_count(machine_code, &counters);
                    /*completes the label coding in the machine code and produces .ext .ent*/
                    second_pass(machine_code, temp_machine_code, symbol_table, source, &counters, &error_found,
                                file_name_without_type);
                } else {
                    fclose(machine_code);
                    remove_output_files(file_name_without_type);
                }
                /**/
                free_symbol_table(symbol_table);
                fclose(temp_machine_code);
                remove_temp_file();
                /**/
                if (error_found == TRUE) {
                    print_errors_summary(file_name_without_type, counters.error_number);
                }
            } else {
                printf("%s is not an .as file. ignored\n", *(argv + i));
            }
        } else {
            printf("%s can't be opened. ignored\n", *(argv + i));
        }
        /*resets the error flag for the next file*/
        error_found = FALSE;
        i++;

    }
    return 0;
}