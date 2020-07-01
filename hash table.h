#ifndef ASSEMBLER_HASH_TABLE_H
#define ASSEMBLER_HASH_TABLE_H
typedef struct {
    char* key;
    void* item;
    void* next;
}hash_item;

int generate_hash_code(hash_item to_generate);
hash_item* create_hash_item(char* key, void* item);
void hash_insert(hash_item to_insert);
hash_item* hash_item_alloc(int size);
#endif /*ASSEMBLER_HASH_TABLE_H*/
