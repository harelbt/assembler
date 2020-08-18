#ifndef ASSEMBLER_ASSEMBLER_DATA_TYPES_H
#define ASSEMBLER_ASSEMBLER_DATA_TYPES_H

/*LENGTHS*/
#define LABEL_MAX_LENGTH 32
#define ORDER_MAX_LENGTH 7
#define OPERATOR_MAX_LENGTH 5

/*FLAGS*/
#define TRUE 1
#define FALSE 0
#define NOT_FOUND -1
#define EXTERN 1
#define CODE 1
#define DATA 2

/*STRUCTS*/
/*represent a symbol/label*/
typedef struct __attribute__((packed)) symbol_table{
    char name[LABEL_MAX_LENGTH];
    int address;
    char external;/*if external, have the value of EXTERN (1)*/
    char sentence_type;/*instruction / order*/
    struct symbol_table* next;
} symbol;
/*used in the second pass to code symbols*/
typedef struct __attribute__((packed)) {
    unsigned int label_address_binary: 24;
} label_address;
/*represent an operator*/
typedef struct{
    char operator_name[OPERATOR_MAX_LENGTH];
    int opcode;
    int function;
}operator;
/*represent an order*/
typedef struct __attribute__((packed)){
    char order[ORDER_MAX_LENGTH];
}data;
/**/
typedef struct __attribute__((packed)){
    char is_comment;
    char is_empty_line;
    char is_code;
    char is_data;
}line_flags;
/*represent a line, contains several structs*/
typedef struct{
    char* line;
    int length;
    symbol label;
    operator code_parts;
    data data_parts;
    line_flags flags;
}line;
/*contains indexes of line components*/
typedef struct __attribute__((packed)){
    int first_char_index;
    int colon_index;
    int semicolon_index;
    int dot_index;
    int first_register_index;
    int second_register_index;
    int first_quotation_mark_index;
    int second_quotation_mark_index;
    int last_comma_index;
    int operator_index;
    int first_operand_index;
    int second_operand_index;
    int data_index;
}line_indexes;
/*contains all the relevant counters of the program*/
typedef struct __attribute__((packed)){
    int IC;
    int last_IC;
    int DC;
    int last_DC;
    int number_of_registers;
    int number_of_colons;
    int number_of_quotation_marks;
    int number_of_dots;
    int number_of_operators;
    int line_number;
    int error_number;
    int number_of_operands;
    int number_of_commas;
}line_counters;
/*represent a word of memory */
typedef struct __attribute__((packed)){
    unsigned int word: 24;
    char is_jump;/*TRUE when it's a "jump to label" (&)*/
    char is_label;/*TRUE when it's a label*/
    int label_index;/*label index in the current line*/
}word;
/*data image node*/
typedef  struct __attribute__((packed)) data_binary_node{
    int DC;
    word word;
    char is_head_filled;/*flag for filling the first node instead of inserting after it*/
    struct data_binary_node * next;
}data_image;

/*FUNCTIONS DECLARATION*/
/**
 * initializes line variables for next line processing.
 * line tools are structs that supply counters, indexes, flags, lengths etc... for a line processing
 * @param sentence pointer to a struct line
 * @param counters pointer to a struct line_counters
 * @param indexes pointer to a struct line_indexes
 */
void initialize_line_variables(line* sentence, line_counters* counters, line_indexes* indexes);
/**
 * initializes counters for next line processing.
 * @param counters pointer to a struct line_counters
 */
static void initialize_counters(line_counters* counters);
/**
 * initializes indexes for next line processing.
 * @param indexes pointer to a struct line_indexes
 */
static void initialize_indexes(line_indexes* indexes);
/**
 * initializes struct line for next line processing.
 * @param sentence pointer to a struct line
 */
static void initialize_line(line* sentence);
/**/
#endif /*ASSEMBLER_ASSEMBLER_DATA_TYPES_H*/
