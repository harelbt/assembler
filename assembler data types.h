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
#define INSTRUCTION 1
#define ORDER 2
#define EXTERN 1
#define ENTRY 2
/*STRUCTS*/
typedef struct __attribute__((packed)) symbol_table{
    char name[LABEL_MAX_LENGTH];
    int address;
    char sentence_type;
    char extern_or_entry;
    struct symbol_table* next;
} symbol;
typedef struct{
    char operator_name[OPERATOR_MAX_LENGTH];
    int opcode;
    int function;
}code;
typedef struct __attribute__((packed)){
    char order[ORDER_MAX_LENGTH];
    char* data;
}data;
typedef struct __attribute__((packed)){
    char is_comment;
    char is_empty_line;
    char is_code;
    char is_data;
}line_flags;
typedef struct{
    char* line;
    int length;
    symbol label;
    code code_parts;
    data data_parts;
    line_flags flags;
}line;
typedef struct __attribute__((packed)){
    int first_char_index;
    int colon_index;
    int semicolon_index;
    int first_hash_mark_index;
    int second_hash_mark_index;
    int dot_index;
    int first_register_index;
    int second_register_index;
    int first_quotation_mark_index;
    int second_quotation_mark_index;
    int operator_index;
    int first_operand_index;
    int second_operand_index;
    int data_index;
}line_indexes;
typedef struct __attribute__((packed)){
    int IC;
    int last_instruction_address;
    int DC;
    int last_data_address;
    int number_of_registers;
    int number_of_hashmarks;
    int number_of_colons;
    int number_of_quotation_marks;
    int number_of_dots;
    int number_of_operators;
    int line_number;
    int error_number;
    int number_of_operands;
    int number_of_commas;
}line_counters;
typedef struct __attribute__((packed)){
    unsigned int word: 24;
    char is_jump;
    char is_label;
    int label_index;
}word;
typedef  struct __attribute__((packed)) data_binary_node{
    int DC;
    word word;
    char is_head_filled;
    struct data_binary_node * next;
}data_image;
/*FUNCTIONS DECLARATION*/
void initialize_line_tools(line* sentence, line_counters* counters, line_indexes* indexes);
void initialize_counters(line_counters* counters);
void initialize_indexes(line_indexes* indexes);
void initialize_line(line* sentence);
/**/
#endif /*ASSEMBLER_ASSEMBLER_DATA_TYPES_H*/
