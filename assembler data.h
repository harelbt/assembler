#ifndef ASSEMBLER_ASSEMBLER_DATA_H
#define ASSEMBLER_ASSEMBLER_DATA_H
#define EXTERN 1
#define ENTRY 0
#define ORDER 1
#define INSTRUCTION 0
#define LABEL_MAX_LENGTH 31
typedef struct {
    char name[LABEL_MAX_LENGTH];
    char* address;
    char sentence_type;
    char extern_or_entry;
} symbol;
typedef struct {
    char* line;
    symbol label;
    char* operator;
    char* first_operand;
    char* second_operand;
    short int is_comment;
    int line_number;
}line;
#endif /*ASSEMBLER_ASSEMBLER_DATA_H*/
