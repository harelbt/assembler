#ifndef ASSEMBLER_ASSEMBLER_DATA_H
#define ASSEMBLER_ASSEMBLER_DATA_H
#define EXTERN 1
#define ENTRY 0
#define ORDER 1
#define INSTRUCTION 0
#define LABEL_MAX_LENGTH 31
int IC, DC;
struct {
    char name[LABEL_MAX_LENGTH];
    char* address;
    char sentence_type;
    char extern_or_entry;
}symbol;

void initialize_all();
#endif //ASSEMBLER_ASSEMBLER_DATA_H
