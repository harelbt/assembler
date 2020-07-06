
#ifndef ASSEMBLER_ERRORS_H
#define ASSEMBLER_ERRORS_H
#define UNEXPECTED_SEMICOLON 1
#define UNEXPECTED_HASHMARK 2
#define UNEXPECTED_REGISTER 3
#define UNEXPECTED_QUOT_MARK 4
#define REGISTER_NO_OPERATOR 5
#define HASHMARK_NO_OPERATOR 6
#define EXTRA_OPERATORS 7
#include "assembler data types.h"

void report_error(line* sentence, short int error_code);
void inspect_non_instruction_line(line* sentence, line_marks_index indexes);
#endif /*ASSEMBLER_ERRORS_H*/
