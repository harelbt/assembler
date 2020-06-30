#include <stdlib.h>
#include "assembler data.h"

void initialize_all(){
    IC = 100;
    DC = 0;
    symbol_table = NULL;
    data_table = NULL;
}