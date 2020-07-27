#ifndef ASSEMBLER_ASSEMBLER_DATA_TYPES_H
#define ASSEMBLER_ASSEMBLER_DATA_TYPES_H
#define EXTERN 1
#define ENTRY 0
#define ORDER 1
#define INSTRUCTION 0
#define LABEL_MAX_LENGTH 32
#define ORDER_MAX_LENGTH 7
#define OPERATOR_MAX_LENGTH 5
#define TRUE 1
#define FALSE 0
typedef struct {
    char name[LABEL_MAX_LENGTH];
    char* address;
    char sentence_type;
    char extern_or_entry;
} symbol;
typedef struct {
    char operator_name[OPERATOR_MAX_LENGTH];
    int opcode;
    int function;
}operator;
typedef struct {
    char* first_operand;
    char* second_operand;
    operator operator_parts;
}code;
typedef struct {
    char order[ORDER_MAX_LENGTH];
    char* data;
}data;
typedef struct {
    short int is_comment;
    short int is_empty_line;
    short int is_code;
    short int is_data;
}line_flags;
typedef struct {
    char* line;
    int length;
    symbol label;
    code code_parts;
    data data_parts;
    line_flags flags;
}line;
typedef struct {
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
    int first_label_operator_index;
    int second_label_operator_index;
    int data_index;
}line_marks_index;
typedef struct {
    int number_of_registers;
    int number_of_hashmarks;
    int number_of_colons;
    int number_of_quotation_marks;
    int number_of_dots;
    int number_of_operators;
    int line_number;
    int error_number;
    int number_of_operands;
}line_marks_counter;
static void create_symbol(symbol* to_initialize, char* name, char* address, char sentence_type, char extern_or_entry);
void initialize_line_tools(line* sentence, line_marks_counter* counters, line_marks_index* indexes);
void initialize_counters(line_marks_counter* counters);
void initialize_indexes(line_marks_index* indexes);
void initialize_line(line* sentence);
#endif /*ASSEMBLER_ASSEMBLER_DATA_TYPES_H*/
