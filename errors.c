#include "errors.h"
#include "helpfunctions.h"
void report_error(line* sentence, short int error_code){
    printf("ERROR: ");
    switch (error_code) {
        case UNEXPECTED_SEMICOLON:{
            printf("unexpected semicolon in line %d.\n", sentence->line_number);
            break;
        }
        case UNEXPECTED_HASHMARK:{
            printf("unexpected hash mark in line %d.\n", sentence->line_number);
            break;
        }
        case UNEXPECTED_REGISTER:{
            printf("unexpected register in line %d.\n", sentence->line_number);
            break;
        }
        case UNEXPECTED_QUOT_MARK:{
            printf("unexpected quotation mark in line %d.\n", sentence->line_number);
            break;
        }
        case REGISTER_NO_OPERATOR:{
            printf("register is written but no operator found in line %d.\n", sentence->line_number);
            break;
        }
        case HASHMARK_NO_OPERATOR:{
            printf("hash mark is written but no operator found in line %d.\n", sentence->line_number);
            break;
        }
        case EXTRA_OPERATORS:{
            printf("Extra operator in line %d.\n", sentence->line_number);
            break;
        }
        default:
            return;
    }
}
void inspect_non_instruction_line(line* sentence, line_marks_index indexes){
    if (indexes.first_register_index >= 0){
        report_error(sentence, REGISTER_NO_OPERATOR);
    }
    if (indexes.first_hash_mark_index >= 0){
        report_error(sentence, HASHMARK_NO_OPERATOR);
    }
}
/*void inspect_instruction_line(line* sentence, line_marks_index indexes){

}*/