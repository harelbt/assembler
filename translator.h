#ifndef ASSEMBLER_TRANSLATOR_H
#define ASSEMBLER_TRANSLATOR_H
#include "assembler data types.h"

/*GENERAL MASKS*/
#define ONES_COMP_MASK 0b111111111111111111111111u

/*opcodes*/
#define OPCODE_ONE 0b000001000000000000000000u
#define OPCODE_TWO 0b000010000000000000000000u
#define OPCODE_FOUR 0b000100000000000000000000u
#define OPCODE_FIVE 0b000101000000000000000000u
#define OPCODE_NINE 0b001001000000000000000000u
#define OPCODE_TWELVE 0b001100000000000000000000u
#define OPCODE_THIRTEEN 0b001101000000000000000000u
#define OPCODE_FOURTEEN 0b001110000000000000000000u
#define OPCODE_FIFTEEN 0b001111000000000000000000u
/*functions*/
#define FUNCTION_ONE 0b000000000000000000001000u
#define FUNCTION_TWO 0b000000000000000000010000u
#define FUNCTION_THREE 0b000000000000000000011000u
#define FUNCTION_FOUR 0b000000000000000000100000u
/*addressing*/
#define FIRST_DEST_ADDRESSING 0b000000000000100000000000u
#define SECOND_DEST_ADDRESSING 0b000000000001000000000000u
#define THIRD_DEST_ADDRESSING 0b000000000001100000000000u
#define FIRST_SOURCE_ADDRESSING 0b000000010000000000000000u
#define SECOND_SOURCE_ADDRESSING 0b000000100000000000000000u
#define THIRD_SOURCE_ADDRESSING 0b000000110000000000000000u
/*registers*/
#define SOURCE_REGISTER_ONE 0b000000000010000000000000u
#define SOURCE_REGISTER_TWO 0b000000000100000000000000u
#define SOURCE_REGISTER_THREE 0b000000000110000000000000u
#define SOURCE_REGISTER_FOUR 0b000000001000000000000000u
#define SOURCE_REGISTER_FIVE 0b000000001010000000000000u
#define SOURCE_REGISTER_SIX 0b000000001100000000000000u
#define SOURCE_REGISTER_SEVEN 0b000000001110000000000000u
#define DEST_REGISTER_ONE 0b000000000000000100000000u
#define DEST_REGISTER_TWO 0b000000000000001000000000u
#define DEST_REGISTER_THREE 0b000000000000001100000000u
#define DEST_REGISTER_FOUR 0b000000000000010000000000u
#define DEST_REGISTER_FIVE 0b000000000000010100000000u
#define DEST_REGISTER_SIX 0b000000000000011000000000u
#define DEST_REGISTER_SEVEN 0b000000000000011100000000u
/*ARE*/
#define ABSOLUTE 0b000000000000000000000100u
#define RELOCATABLE 0b000000000000000000000010u
#define EXTERNAL 0b000000000000000000000001u
/*END OF MASKS*/

/*FLAGS*/
#define CODE 1
#define DATA 2

/*GENERAL*/
#define ONE_WORD 1
#define NUMBER_ALLOWED_LENGTH 8
#define COMPLEMENT_TO_TWO 1
/*general*/
void first_pass_translation(FILE* machine_code, line* sentence, line_indexes* indexes, line_counters* counters, data_image* data);
/*instruction translation functions*/
static void code_translation(FILE* machine_code, line* sentence, line_indexes* indexes, line_counters* counters);
/*____________________________________________________________________________________________________________________*/
static void prepare_instruction_word(word* to_prepare, line* sentence, line_indexes* indexes);
/**/static void prepare_dest_only_instruction(word* to_prepare, const char* line, line_indexes* indexes);
/**/static void prepare_full_instruction_word(word* to_prepare, line_indexes* indexes, const char* line);
/*____________________________________________________________________________________________________________________*/
static void prepare_extra_words(const char* line, line_indexes* indexes, int num_of_words, ...);
static void code_word( word* word, const char* line, int index, line_indexes* indexes, char mode);
static void code_instruction_word(word* word, const char* line, int index, line_indexes* indexes);
/*data translation functions*/
static void data_translation(line* sentence, line_indexes* indexes, line_counters* counters, data_image* data);
static void translate_numbers_sequence(const char* numbers_sequence, line_indexes* indexes, int last_DC, data_image* data);
static void translate_string_sequence(const char* string_sequence, int last_DC, int second_quotemark_index, data_image* data);
static void translate_character(data_image* new_data, char character, int last_DC, int index);
static char* get_number(const char* line, int index);
static void get_to_next_number(int* index, const char* line);
static void code_number(word* word, const char* line, int index, char mode);
void insert_data_node(data_image* head, data_image* to_insert);
/*set instruction bitfield functions*/
static void set_opcode(word* word, int opcode);
static void set_function(word* word, int function);
static void set_dest_addressing(word* word, const char* line, line_indexes* indexes);
static void set_source_addressing(const char* line, word* word, line_indexes* indexes);
static void set_source_register(word* word, const char* line, int register_index);
static void set_dest_register(word* word, const char* line, int register_index);
/*addressing discover functions*/
static int get_dest_addressing(const char* line, line_indexes* indexes);
static int get_source_addressing(const char* line, line_indexes* indexes);
/*words calculation*/
void calculate_number_of_words(line* sentence, line_indexes indexes, line_counters* counters);
void calculate_instruction_word(line* sentence, line_indexes indexes, line_counters* counters);
void calculate_order_word(line* sentence, line_indexes indexes, line_counters* counters);
#endif //ASSEMBLER_TRANSLATOR_H
