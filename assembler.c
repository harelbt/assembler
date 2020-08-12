#include <time.h>
#include "assembler.h"
#include "in out tools.h"
int main (int argc, char* argv[]){
    char error_found = FALSE;
    int i = 1;
    symbol* symbol_table;
    symbol* symbol_addresses_table[NUM_OF_ENGLISH_CHARS];
    FILE* machine_code;
    FILE *filep;
    char* name_without_type;
    clock_t start = clock();
    clock_t end;
    double runtime;
    /*checks if operators supplied*/
    if (argc == 1) {
        /*prints this error to stderr with exit code 1*/
        stop(EXIT_FAILURE, "no assembly code was supplied");
    }
    /*assembles each supplied file*/
    while (i < argc) {
        /*opening file*/
        filep = open_file(*(argv+i), "r");/*this custom function can handle malloc failure*/
        name_without_type = get_file_name_without_type(*(argv + i));
        machine_code = open_machine_code(name_without_type);
        symbol_table = first_pass(filep, *(argv + i), machine_code, symbol_addresses_table, &error_found);

        fclose(machine_code);
        if (error_found == TRUE) {
            remove_output_files(name_without_type);
        }
        error_found = FALSE;/*resets error flag for the next file*/
        i++;
    }
    while (symbol_table){
        printf("%s %d\n", symbol_table->name, symbol_table->address);
        symbol_table = symbol_table->next;
    }
    int a = NUM_OF_ENGLISH_CHARS;
    symbol* s;
    while (a!=-1){
        s = *(symbol_addresses_table + a);
        if (a == 'D' - 65){
            printf("~~~~~%s\n", s->name);
        }
        a--;
    }
    end = clock();
    runtime = (double)(end - start)/CLOCKS_PER_SEC;
    printf("%f", runtime);
    return 0;
}
static char* get_file_name_without_type(char* file_name){
    unsigned int length = strlen(file_name);
    char* i = file_name + length;
    while (*i != '.'){
        i--;
    }
    *i = '\0';
    return file_name;
}
static void remove_output_files(char* file_name){
    remove_ob_file(file_name);
    //remove_ent_file(file_name);
    //remove_ext_file(file_name);
}
static void remove_ob_file(char* file_name){
    char to_remove [strlen(file_name) + TYPE_MAX_LENGTH];
    strcpy(to_remove, file_name);
    strcat(to_remove, ".ob");
    remove(to_remove);
}
void remove_ent_file(char* file_name){
    char to_remove [strlen(file_name) + TYPE_MAX_LENGTH];
    strcpy(to_remove, file_name);
    strcat(to_remove, ".ext");
    remove(to_remove);
}
static void remove_ext_file(char* file_name){
    char to_remove [strlen(file_name) + TYPE_MAX_LENGTH];
    strcpy(to_remove, file_name);
    strcat(to_remove, ".ent");
    remove(to_remove);
}
static FILE* open_machine_code(char* file_name){
    char to_open[strlen(file_name) + TYPE_MAX_LENGTH];
    FILE* machine_code;
    strcpy(to_open, file_name);
    strcat(to_open, ".ob");
    machine_code = open_file(to_open, "a+");
    return machine_code;
}