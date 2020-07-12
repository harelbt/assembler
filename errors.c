#include "errors.h"
#include "helpfunctions.h"
void report_error(line sentence, short int error_code, line_marks_counter counters){
    printf("ERROR: ");
    switch (error_code) {
        case UNEXPECTED_SEMICOLON:{
            printf("unexpected semicolon in line %d.\n", counters.line_number);
            break;
        }
        case UNEXPECTED_HASHMARK:{
            printf("unexpected hash mark in line %d.\n", counters.line_number);
            break;
        }
        case UNEXPECTED_REGISTER:{
            printf("unexpected register in line %d.\n", counters.line_number);
            break;
        }
        case UNEXPECTED_QUOT_MARK:{
            printf("unexpected quotation mark in line %d.\n", counters.line_number);
            break;
        }
        case REGISTER_NO_OPERATOR:{
            printf("register is written but no operator found in line %d.\n", counters.line_number);
            break;
        }
        case HASHMARK_NO_OPERATOR:{
            printf("hash mark is written but no operator found in line %d.\n", counters.line_number);
            break;
        }
        case EXTRA_OPERATORS:{
            printf("Extra operator in line %d.\n", counters.line_number);
            break;
        }
        case MIXED_SENTENCE:{
            printf("Mixed code and data order in line %d.\n", counters.line_number);
            break;
        }
        case NO_SENTENCE_TYPE:{
            printf("Sentence is not code or data order in line %d.\n", counters.line_number);
            break;
        }
        default:
            return;
    }
    puts(sentence.line);
}
short int errors_inspection(line sentence, line_marks_index indexes, line_marks_counter counters){
    short int error_found = 0;
    if (sentence.flags.is_data == TRUE && sentence.flags.is_code == TRUE){
        report_error(sentence, MIXED_SENTENCE, counters);
        error_found = 1;
    } else if (sentence.flags.is_data == TRUE){
        error_found = inspect_non_code_line(sentence,indexes, counters);
    } else if(sentence.flags.is_code == TRUE){
        error_found = inspect_code_line(sentence, indexes, counters);
    } if (sentence.flags.is_data == FALSE && sentence.flags.is_code == FALSE){
        report_error(sentence, NO_SENTENCE_TYPE, counters);
        error_found = 1;
    }
    return error_found;
}
short int inspect_non_code_line(line sentence, line_marks_index indexes, line_marks_counter counters){
    short int error_found = 0;
    if (counters.number_of_registers > 0){
        report_error(sentence, REGISTER_NO_OPERATOR, counters);
    }
    if (counters.number_of_hashmarks > 0){
        report_error(sentence, HASHMARK_NO_OPERATOR, counters);
    }
    if (counters.number_of_quotation_mark )
    return error_found;
}
short int inspect_code_line(line sentence, line_marks_index indexes, line_marks_counter counters){

}