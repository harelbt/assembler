#ifndef ASSEMBLER_ASSEMBLER_DATA_TYPES_H
#define ASSEMBLER_ASSEMBLER_DATA_TYPES_H
#define EXTERN 1
#define ENTRY 0
#define ORDER 1
#define INSTRUCTION 0
#define LABEL_MAX_LENGTH 31
#define TRUE 1
#define FALSE 0
typedef struct {
    char name[LABEL_MAX_LENGTH];
    char* address;
    char sentence_type;
    char extern_or_entry;
} symbol;
typedef struct {
    int opcode;
    int function;
    char* first_operand;
    char* second_operand;
}code;
typedef struct {
    char order[7];
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
}line_marks_index;
typedef struct {
    int number_of_registers;
    int number_of_hashmarks;
    int number_of_colons;
    int number_of_quotation_mark;
    int number_of_dots;
    int number_of_operators;
    int line_number;
}line_marks_counter;
typedef struct {
    char operator_name[5];
    int recognized_opcode;
    int recognized_function;
}operator;
static void create_symbol(symbol* to_initialize, char* name, char* address, char sentence_type, char extern_or_entry);
void initialize_line_tools(line* sentence, line_marks_counter* counters, line_marks_index* indexes);
void initialize_operator_variables(operator* op_variables);
void initialize_counters(line_marks_counter* counters);
void initialize_indexes(line_marks_index* indexes);
void initialize_line(line* sentence);
#endif /*ASSEMBLER_ASSEMBLER_DATA_TYPES_H*/
