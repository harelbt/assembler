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
void initialize_all(int* IC, int* DC, symbol* symbol_table, int* data_table, long* symbol_tabel_length);
#endif /*ASSEMBLER_ASSEMBLER_DATA_H*/
