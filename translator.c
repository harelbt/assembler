#include <stdio.h>
#include "translator.h"
#include "first pass.h"
void first_pass_translation(FILE* machine_code, line* sentence, line_indexes* indexes, line_counters* counters){
    if (sentence->flags.is_code == TRUE){
        code_translation(machine_code, sentence, indexes, counters);
    } else{
        data_translation(sentence, indexes, counters);
    }
}
static void code_translation(FILE* machine_code, line* sentence, line_indexes* indexes, line_counters* counters){
    instruction_word instruction_word;
    word second_word;
    word third_word;
    int distance_from_last_IC = counters->IC - counters->last_instruction_address;
    prepare_instruction_word(&instruction_word, sentence, indexes);

}
/*This function resets a string to a string that contains only \0 characters*/
void resetString(char str[100]){
    int i;
    for (i = 0; i <100; ++i)
        str[i]='\0';
}
int isWhitespace(char letter){
    if(letter == ' ' || letter =='\t')
        return 1;
    else
        return 0;
}
static struct data_binary_node * data_translation(line* sentence, line_indexes* indexes, line_counters* counters) {
    char *data = sentence->data_parts.data;
    char temp[100];
    int j = 0;
    long num;
    char *eptr;
    int dc = counters->last_data_address;
    static struct data_binary_node *ptr = NULL;
    static struct data_binary_node *head = NULL;
    resetString(temp);
    while (isWhitespace(*data))
        data++;
    while (*data != '\n') {
        if (indexes->first_quotation_mark_index != NOT_FOUND) {/*the data is a string*/
            if (*data == '"') {
                data++;
                unsigned int ascii_letter = *data;
                if (ptr == NULL) {/*its the first node*/
                    head = (struct data_binary_node *) malloc(sizeof(struct data_binary_node));
                    head->translated = ascii_letter;
                    head->dc = dc;
                    head->next = NULL;
                    ptr = head;
                    data++;
                    dc++;
                }
                /*adding the other nodes to the list*/
                while (*data != '"') {
                    struct data_binary_node *node_string = NULL;
                    node_string = (struct data_binary_node *) malloc(sizeof(struct data_binary_node));
                    ascii_letter = *data;
                    node_string->translated = ascii_letter;
                    node_string->dc = dc;
                    while (ptr->next != NULL)
                        ptr = ptr->next;
                    ptr->next = node_string;
                    node_string->translated = ascii_letter;
                    ptr->next = node_string;
                    node_string->next = NULL;
                    data++;
                    dc++;
                }
            }
        } else {/*the data is integers*/
            while(*data == '-'||(*data <= '9' && *data >= '0')) {
                temp[j] = *data;
                j++;
                data++;
            }
            /*finished getting the number*/

            num = (unsigned int)strtol(temp, &eptr, 2);
            unsigned int num2 = (unsigned int) num;
            if (num2 < 0) {
                negate_to_binary(num2);
            }
            /*finished translating the number to binary*/
            resetString(temp);
            j = 0;
            if (ptr == NULL) {//its the first node
                static struct data_binary_node *head;
                head = (struct data_binary_node *) malloc(sizeof(struct data_binary_node));
                head->translated = num2;
                head->dc = dc;
                head->next = NULL;
                ptr = head;
                dc++;
            }
            else {/*adding the node to the list*/
                struct data_binary_node *node_num = NULL;
                node_num = (struct data_binary_node *) malloc(sizeof(struct data_binary_node));
                while (ptr->next != NULL)
                    ptr = ptr->next;
                node_num->translated = num2;
                node_num->dc = dc;
                ptr->next = node_num;
                node_num->next = NULL;
                dc++;
            }
            data++;
        }
    }
    return head;
}
static void prepare_instruction_word(instruction_word* to_prepare, line* sentence, line_indexes* indexes){
    to_prepare->ARE = 0b100;
    to_prepare->opcode = sentence->code_parts.opcode;
    to_prepare->function = sentence->code_parts.function;
    to_prepare->source_register = indexes->first_operand_index == indexes->first_register_index ?
                                 *(sentence->line + indexes->first_register_index + 1) : 0b0;
    to_prepare->dest_register = indexes->second_operand_index == indexes->second_register_index ?
                               *(sentence->line + indexes->second_register_index + 1) : 0b0;
}
static void prepare_extra_words(const char* line, line_indexes indexes, int num_of_words, ...){
    va_list arg_pointer;
    va_start(arg_pointer, num_of_words);
    if (num_of_words == 2){
        word second_word = va_arg(arg_pointer, word);

    } else{
        word second_word = va_arg(arg_pointer, word);
        word third_word = va_arg(arg_pointer, word);
    }
    va_end(arg_pointer);
}
static void code_word(word word, const char* line, int index){
    switch (*(line + index)) {
        case '#':{}
        case '&':{}
        default:{}
    }
}
static void code_number(word* word, const char* line, int index){
    index++;
    char* to_code;
    switch (*(line + index)) {
        case '-':{
            index++;
            to_code = get_until_white_char(line, index);
            word->number.number = strtod(to_code, NULL);
            word->number.ARE = ABSOLUTE;
        }
        case '+':{
        }
        default:{}
    }
}