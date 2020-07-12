
#ifndef ASSEMBLER_ERRORS_H
#define ASSEMBLER_ERRORS_H
#define UNEXPECTED_SEMICOLON 1
#define UNEXPECTED_HASHMARK 2
#define UNEXPECTED_REGISTER 3
#define UNEXPECTED_QUOT_MARK 4
#define REGISTER_NO_OPERATOR 5
#define HASHMARK_NO_OPERATOR 6
#define EXTRA_OPERATORS 7
#define MIXED_SENTENCE 8
#define NO_SENTENCE_TYPE 9
#include "assembler data types.h"

void report_error(line sentence, short int error_code, line_marks_counter counters);
short int inspect_non_code_line(line sentence, line_marks_index indexes, line_marks_counter counters);
short int inspect_code_line(line sentence, line_marks_index indexes, line_marks_counter counters);
short int errors_inspection(line sentence, line_marks_index indexes, line_marks_counter counters);
#endif /*ASSEMBLER_ERRORS_H*/
