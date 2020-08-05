
#define STRING_LEN 50
#define E 1
#define R 2
#define A 4
typedef struct label_position{
    unsigned int BINARY_LABEL:21;
    unsigned int ARE:3;
}label_address;
int second_pass(FILE * first_pass_file,symbol * symbol_table,FILE * input_file);
void resetString(char str[STRING_LEN]);