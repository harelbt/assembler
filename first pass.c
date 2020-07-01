#include "first pass.h"
int first_pass(char* file){
    FILE* filep = open_file(file,"r");
    while (filep != EOF) {
        read_line(filep);
    }
}
read_line(FILE* file){

}