#ifndef ASSEMBLER_TRANSLATOR_H
#define ASSEMBLER_TRANSLATOR_H
#include "assembler data types.h"
#include "words.h"
void first_pass_translation(FILE* machine_code, line* sentence, line_indexes* indexes, line_counters* counters);
static void code_translation(FILE* machine_code, line* sentence, line_indexes* indexes, line_counters* counters);
static struct data_binary_node * data_translation(line* sentence, line_indexes* indexes, line_counters* counters);
static void prepare_instruction_word(instruction_word* to_prepare, line* sentence, line_indexes* indexes);
static void prepare_extra_words(const char* line, line_indexes indexes, int num_of_words, ...);
#endif //ASSEMBLER_TRANSLATOR_H
