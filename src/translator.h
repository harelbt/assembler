#ifndef ASSEMBLER_TRANSLATOR_H
#define ASSEMBLER_TRANSLATOR_H
#include "assembler data types.h"

/*GENERAL MASKS*/
#define ONES_COMP_MASK 0xFFFFFFu/*0b111111111111111111111111u*/

/*opcodes*/
#define OPCODE_ONE 0x40000u/*0b000001000000000000000000u*/
#define OPCODE_TWO 0x80000u/*0b000010000000000000000000u*/
#define OPCODE_FOUR 0x100000u/*0b000100000000000000000000u*/
#define OPCODE_FIVE 0x140000u/*0b000101000000000000000000u*/
#define OPCODE_NINE 0x240000u/*0b001001000000000000000000u*/
#define OPCODE_TWELVE 0x300000u/*0b001100000000000000000000u*/
#define OPCODE_THIRTEEN 0x340000u/*0b001101000000000000000000u*/
#define OPCODE_FOURTEEN 0x380000u/*0b001110000000000000000000u*/
#define OPCODE_FIFTEEN 0x3C0000u/*0b001111000000000000000000u*/
/*functions*/
#define FUNCTION_ONE 0x8u/*0b000000000000000000001000u*/
#define FUNCTION_TWO 0x10u/*0b000000000000000000010000u*/
#define FUNCTION_THREE 0x18u/*0b000000000000000000011000u*/
#define FUNCTION_FOUR 0x20u/*0b000000000000000000100000u*/
/*addressing*/
#define FIRST_DEST_ADDRESSING 0x800u/*0b000000000000100000000000u*/
#define SECOND_DEST_ADDRESSING 0x1000u/*0b000000000001000000000000u*/
#define THIRD_DEST_ADDRESSING 0x1800u/*0b000000000001100000000000u*/
#define FIRST_SOURCE_ADDRESSING 0x10000u/*0b000000010000000000000000u*/
#define SECOND_SOURCE_ADDRESSING 0x20000u/*0b000000100000000000000000u*/
#define THIRD_SOURCE_ADDRESSING 0x30000u/*0b000000110000000000000000u*/
/*registers*/
#define SOURCE_REGISTER_ONE 0x2000u/*0b000000000010000000000000u*/
#define SOURCE_REGISTER_TWO 0x4000u/*0b000000000100000000000000u*/
#define SOURCE_REGISTER_THREE 0x6000u/*0b000000000110000000000000u*/
#define SOURCE_REGISTER_FOUR 0x8000u/*0b000000001000000000000000u*/
#define SOURCE_REGISTER_FIVE 0xA000u/*0b000000001010000000000000u*/
#define SOURCE_REGISTER_SIX 0xC000u/*0b000000001100000000000000u*/
#define SOURCE_REGISTER_SEVEN 0xE000u/*0b000000001110000000000000u*/
#define DEST_REGISTER_ONE 0x100u/*0b000000000000000100000000u*/
#define DEST_REGISTER_TWO 0x200u/*0b000000000000001000000000u*/
#define DEST_REGISTER_THREE 0x300u/*0b000000000000001100000000u*/
#define DEST_REGISTER_FOUR 0x400u/*0b000000000000010000000000u*/
#define DEST_REGISTER_FIVE 0x500u/*0b000000000000010100000000u*/
#define DEST_REGISTER_SIX 0x600u/*0b000000000000011000000000u*/
#define DEST_REGISTER_SEVEN 0x700u/*0b000000000000011100000000u*/
/*ARE*/
#define ABSOLUTE 0x4u/*0b000000000000000000000100u*/
#define RELOCATABLE 0x2u/*0b000000000000000000000010u*/
#define EXTERNAL 0x1u/*0b000000000000000000000001u*/
/*END OF MASKS*/

/*FLAGS*/
#define CODE 1
#define DATA 2

/*GENERAL*/
#define ONE_WORD 1
#define NUMBER_ALLOWED_LENGTH 8
#define COMPLEMENT_TO_TWO 1
/*general*/
/**
 * translation for the first pass. translates the source file to a semi ready machine code.
 * labels are printed in ?LABEL format(if the label's name is LABEL).
 * requires the data image head
 * @param machine_code FILE* to code to
 * @param sentence struct line pointer
 * @param indexes struct line_indexes pointer
 * @param counters struct line_counters pointer
 * @param data data image head
 */
void first_pass_translation(FILE* machine_code, line* sentence, line_indexes* indexes, line_counters* counters, data_image* data);
/*instruction translation functions*/
/**
 * FIRST PASS LEVEL
 *translates a code(instruction) line to the machine code file.
 * @param machine_code FILE* to code to
 * @param sentence struct line pointer
 * @param indexes struct line_indexes pointer
 * @param counters struct line_counters pointer
 */
void code_translation(FILE* machine_code, line* sentence, line_indexes* indexes, line_counters* counters);
/*____________________________________________________________________________________________________________________*/
/**
 * FIRST PASS LEVEL
 * codes instruction line to word in a given struct word
 * @param to_prepare word* to code to
 * @param sentence struct line pointer
 * @param indexes struct line_indexes pointer
 */
void prepare_instruction_word(word* to_prepare, line* sentence, line_indexes* indexes);
/**
 * FIRST PASS LEVEL
 * codes a line with only a destination operand
 * @param to_prepare word* to code to
 * @param line string of the line to translate
 * @param indexes struct line_indexes pointer
 */
/**/void prepare_dest_only_instruction(word* to_prepare, const char* line, line_indexes* indexes);
/**
 * FIRST PASS LEVEL
 * codes a line with 2 or 0 operands
 * @param to_prepare word* to code to
 * @param indexes struct line_indexes pointer
 * @param line string of the line to translate
 */
/**/void prepare_full_instruction_word(word* to_prepare, line_indexes* indexes, const char* line);
/*____________________________________________________________________________________________________________________*/
/**
 * FIRST PASS LEVEL
 * codes the extra word of an instruction word.
 * struct word to code to can be given as ... arguments(in the end of the signature)
 * requires a number of words for the line (1-3)
 * @param line string of the line to translate
 * @param indexes struct line_indexes pointer
 * @param counters struct line_counters pointer
 * @param num_of_words num number of words for the line (1-3)
 * @param ... word pointer (max 2) to code to
 */
void prepare_extra_words(const char* line, line_indexes* indexes, line_counters* counters, int num_of_words, ...);
/**
 * FIRST PASS LEVEL
 * codes one operand or one number to one struct word
 * requires a "mode" flag (CODE or DATA macros) to know the type of the coding
 * @param word word* to code to
 * @param line string of the line to translate
 * @param index index of the number or operand (if #, index of #)
 * @param indexes struct line_indexes pointer
 * @param counters struct line_counters pointer
 * @param mode CODE or DATA macros (instruction or order)
 */
void code_word( word* word, const char* line, int index, line_indexes* indexes, line_counters* counters, char mode);
/**
 * FIRST PASS LEVEL
 * codes one operand to one struct word
 * @param word word* to code to
 * @param line string of the line to translate
 * @param index index of the operand
 * @param indexes struct line_indexes pointer
 * @param counters struct line_counters pointer
 */
void code_instruction_word(word* word, const char* line, int index, line_indexes* indexes, line_counters* counters);
/*data translation functions*/
/**
 * FIRST PASS LEVEL
 * translates an order line to the machine code file.
 * @param sentence struct line pointer
 * @param indexes struct line_indexes pointer
 * @param counters struct line_counters pointer
 * @param data data image head
 */
void data_translation(line* sentence, line_indexes* indexes, line_counters* counters, data_image* data);
/**
 * FIRST PASS LEVEL
 * translates a sequence of numbers(after a .data order)
 * requires the number sequence as a string
 * @param numbers_sequence string of the number sequence
 * @param indexes struct line_indexes pointer
 * @param counters struct line_counters pointer
 * @param last_DC the last DC(found in counters struct)
 * @param data data image head
 */
void translate_numbers_sequence(const char* numbers_sequence, line_indexes* indexes, line_counters* counters, int last_DC, data_image* data);
/**
 * FIRST PASS LEVEL
 * translates a sequence of chars(after a .string order)
 * requires the chars sequence as a string
 * @param string_sequence the chars sequence as a string
 * @param last_DC the last DC(found in counters struct)
 * @param second_quotemark_index found "indexes" struct
 * @param data
 */
void translate_string_sequence(const char* string_sequence, int last_DC, int second_quotemark_index, data_image* data);
/**
 * FIRST PASS LEVEL
 * translates a character to a word
 * @param new_data data node to code to
 * @param character to code
 * @param last_DC the last DC(found in counters struct)
 * @param index index of the current characer relatively to the start of the chars sequence
 */
void translate_character(data_image* new_data, char character, int last_DC, int index);
/**
 * FIRST PASS LEVEL
 * returns the string of the number in a string that starts in a given index
 * @param line the string the number is in
 * @param index the start index of the number
 * @return the string of the number
 */
char* get_number(const char* line, int index);
/**
 * FIRST PASS LEVEL
 * gets to the next number in a string and stores the index in the "index" argument
 * @param index index of the current number
 * @param line the string of the numbers
 */
void get_to_next_number(int* index, const char* line);
/**
 * FIRST PASS LEVEL
 * codes one number to a struct word. can code a number with/out # and with signs
 * requires a "mode" flag (CODE/DATA) to know how to work with # and ARE
 * @param word word* to code to
 * @param line string that the number is in
 * @param index index of the number
 * @param mode CODE/DATA (instruction/order)
 */
void code_number(word* word, const char* line, int index, char mode);
/**
 * inserts data image node to the data image
 * @param head data image head
 * @param to_insert data node to insert
 */
void insert_data_node(data_image* head, data_image* to_insert);
/*set instruction bitfield functions*/
/**
 * FIRST PASS LEVEL
 * sets the opcode area in a given struct word's bitfield, according to a given opcode
 * @param word word* to code to
 * @param opcode opcode to code
 */
void set_opcode(word* word, int opcode);
/**
 * FIRST PASS LEVEL
 * sets the function area in a given struct word's bitfield, according to a given function
 * @param word word* to code to
 * @param function function to code
 */
void set_function(word* word, int function);
/**
 * FIRST PASS LEVEL
 * sets the destination addressing in a given struct word's bitfield.
 * requires the code line
 * @param word word* to code to
 * @param line the code line
 * @param indexes struct line_indexes pointer
 */
void set_dest_addressing(word* word, const char* line, line_indexes* indexes);
/**
 * FIRST PASS LEVEL
 * sets the source addressing in a given struct word's bitfield.
 * requires the code line
 * @param line the code line
 * @param word word* to code to
 * @param indexes struct line_indexes pointer
 */
void set_source_addressing(const char* line, word* word, line_indexes* indexes);
/**
 * FIRST PASS LEVEL
 * sets the source register in a given struct word's bitfield.
 * requires the code line and the register index
 * @param word word* to code to
 * @param line the code line
 * @param register_index the source register index
 */
void set_source_register(word* word, const char* line, int register_index);
/**
 * FIRST PASS LEVEL
 * sets the destination register in a given struct word's bitfield.
 * requires the code line and the register index
 * @param word word* to code to
 * @param line the code line
 * @param register_index the destination register index
 */
void set_dest_register(word* word, const char* line, int register_index);
/*addressing discover functions*/
/**
 * FIRST PASS LEVEL
 * returns the destination addressing code according to the code line and indexes
 * @param line the code line
 * @param indexes struct line indexes pointer
 * @return the destination addressing
 */
int get_dest_addressing(const char* line, line_indexes* indexes);
/**
 * FIRST PASS LEVEL
 * returns the source addressing code according to the code line and indexes
 * @param line the code line
 * @param indexes struct line indexes pointer
 * @return
 */
int get_source_addressing(const char* line, line_indexes* indexes);
/*words calculation*/
/**
 * calculates the number of words for the current line.
 * updates the "counters" struct
 * @param sentence struct line pointer
 * @param indexes struct line indexes pointer
 * @param counters struct line counters pointer
 */
void calculate_number_of_words(line* sentence, line_indexes indexes, line_counters* counters);
/**
 * calculates the number of words for an instruction line
 * updates the "counters" struct
 * @param counters struct line counters pointer
 */
void calculate_instruction_word(line_counters* counters);
/**
 * calculates the number of words for an order line
 * updates the "counters" struct
 * @param sentence struct line pointer
 * @param indexes struct line indexes pointer
 * @param counters struct line counters pointer
 */
void calculate_order_word(line* sentence, line_indexes indexes, line_counters* counters);
/*label translation*/
/**
 *  SECOND PASS LEVEL
 * codes a "jump to label" (& label) to the machine code.
 * requires the string from the start of the label
 * if errors found, turns "error_found" to TRUE, else FALSE
 * @param machine_code FILE* to code to
 * @param symbol_table symbol table head
 * @param i a string from the start of the label
 * @param counters struct line counters pointer
 * @param label_address label_address pointer to code the address to
 * @param curr_address current word's addres in a string
 * @param error_found if errors found, turns "error_found" to TRUE, else FALSE
 */
void code_jump(FILE* machine_code, symbol* symbol_table, char* i, line_counters* counters, label_address* label_address, char* curr_address, char* error_found);
/**
 * SECOND PASS LEVEL
 * codes a label to the machine code.
 * requires the string from the start of the label and is external flag to let the program know
 * if a .ext file needs to be removed
 * if errors found, turns "error_found" to TRUE, else FALSE
 * @param machine_code FILE* to code to
 * @param externals_file .ext FILE* to write externals addresses to
 * @param symbol_table symbol table head
 * @param i a string from the start of the label
 * @param counters struct line counters pointer
 * @param label_address label_address pointer to code the address to
 * @param curr_address current word's addres in a string
 * @param error_found if errors found, turns "error_found" to TRUE, else FALSE
 * @param is_external flag (TRUE/FALSE)
 */
void code_label_address(FILE* machine_code, FILE* externals_file, symbol* symbol_table, char* i, line_counters* counters, label_address* label_address, char* curr_address, char* error_found, char* is_external);
#endif /*ASSEMBLER_TRANSLATOR_H*/
