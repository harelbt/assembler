#include <stdio.h>
#include <stdlib.h>
#include "helpfunctions.h"
#include "first pass.h"
#include "second_pass.h"
#include "errors.h"
#include "assembler data types.h"
#include "symbol table.h"
#include "stdlib.h"
#include "string.h"
#include "first pass.h"
#include "assembler.h"
void second_pass(FILE* machine_code,FILE* source, symbol* symbol_table){
    code_symbols(machine_code, symbol_table);
    add_entries();
    prepare_output_files();
}
static void code_symbols(FILE* machine_code, symbol* symbol_table){
    char symbol_to_code[LABEL_MAX_LENGTH];
    char current_address_label[LABEL_MAX_LENGTH];
    int curr_char;
    struct{
        unsigned int label_translated :24;
    } label;
    void * symbolPtr;
    int distance;
    symbol* sym;
    fseek(machine_code, 0, SEEK_SET);
    FILE * start_line = machine_code;
    while ((curr_char = fgetc(machine_code)) != EOF){
        if(fgetc(machine_code)=='\n'){
            start_line = machine_code;
            fscanf(start_line,"%s",current_address_label);
        }
        if(curr_char == '?'){
            fprintf(machine_code," ");//overriding the question mark
            if(fgetc(machine_code) == '&'){//need to put the distance of label
                fprintf(machine_code," ");//overriding the & sign
                fscanf(machine_code,"%s",symbol_to_code);
                symbolPtr = get_symbol(symbol_table,symbol_to_code,&symbol_table);
                sym = (struct symbol_table*)symbolPtr;
                if(strcmp(sym->name,symbol_to_code)){
                    distance = atoi(current_address_label) - sym->address;
                    if(distance<0){
                        distance = abs(distance);
                        label.label_translated = negate_to_binary(distance);
                    }
                    label.label_translated = ((distance << 3)|A);//turn on the A bit
                    fprintf(machine_code,"%06x",label.label_translated);/*write in hexa the label address*/
                }
                else{
                    fprintf(stderr,"The label %s does not exist in the file",symbol_to_code); /*throw an error that there is no label*/
                }
                }
            else{//need to put in  address of the label
                fscanf(machine_code,"%s",symbol_to_code);
                symbolPtr = get_symbol(symbol_table,symbol_to_code,&symbol_table);
                sym = (struct symbol_table*)symbolPtr;
                if(strcmp(sym->name,symbol_to_code)){
                    label.label_translated = sym->address;
                    fprintf(machine_code,"%06x",label.label_translated);/*write in hexa the label address*/

                }
                else{
                    fprintf(stderr,"The label %s does not exist in the file",symbol_to_code);/* throw an error that there is no label with such name*/
                }
            }
            while((fgetc(machine_code)) != '\n'){//deleting all the writing that is not part of the translated label
                fprintf(machine_code," ");
            }
        }

    }
}

static void add_entries(){}
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
