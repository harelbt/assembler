#include <stdio.h>
#include <stdlib.h>
#include "helpfunctions.h"
#include "first pass.h"
#include "second_pass.h"
second_pass(FILE * first_pass_file,symbol * symbol_table,FILE * input_file){
    char[STRING_LEN] string;
int i,distance;
    label_address labelAddress1;
    symbol label1,label2;
    FILE * externs,entries;
fopen(first_pass_file,"a+");
fopen(input_file,"r");

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
}

}
void resetString(char str[STRING_LEN]){
    int i;
    for (i = 0; i <STRING_LEN; ++i)
        str[i]='\0';
}