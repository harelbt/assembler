#include "words.h"
#include "helpfunctions.h"
#include "assembler data.h"
#include "first pass.h"
int main (int argc, char* argv[]){
    int i = 0;
    if (argc == 1) {
        stop(EXIT_FAILURE, "no assembly code was supplied");
    }
    while (i < argc) {
        initialize_all();
        first_pass(argv[i]);
        /*second_pass();*/
        i++;
    }
    return 0;
}