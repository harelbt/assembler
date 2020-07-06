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
}instruction;
typedef struct {
    char* order;
    char* data;
}order;
typedef struct {
    short int is_comment;
    short int is_empty_line;
    short int is_instruction;
    short int is_order;
}line_flags;
typedef struct {
    char* line;
    symbol label;
    instruction code_parts;
    order data_parts;
    line_flags flags;
    int line_number;
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
    int first_quotation_mark;
    int second_quotation_mark;
}line_marks_index;
static void create_symbol(symbol* to_initialize, char* name, char* address, char sentence_type, char extern_or_entry);
#endif /*ASSEMBLER_ASSEMBLER_DATA_TYPES_H*/
