#include <stdlib.h>
#include "helpfunctions.h"
#include "first pass.h"
int main (int argc, char* argv[]){
    int IC = 100;
    int DC = 0;
    char error_found = FALSE;
    int i = 1;
    /*checks if operators supplied*/
    if (argc == 1) {
        /*prints this error to stderr with exit code 1*/
        stop(EXIT_FAILURE, "no assembly code was supplied");
    }
    /*assembles each supplied file*/
    while (i < argc) {
        first_pass(*(argv+i), &error_found);
        if (!error_found) {
            /*second_pass();*/
        }
        error_found = FALSE;/*resets error flag for the next file*/
        i++;
    }
    return 0;
}