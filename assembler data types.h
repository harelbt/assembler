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
    char* operator;
    char* first_operand;
    char* second_operand;
}instruction;
typedef struct {
    char* order;
    /**/
}order;
typedef struct {
    char* line;
    symbol label;
    instruction instruction_parts;
    short int is_comment;
    short int is_empty_line;
    int line_number;
}line;
typedef struct error_message{
    char* error;
    int line;
    struct error_message* next;
}error;
#endif /*ASSEMBLER_ASSEMBLER_DATA_TYPES_H*/
