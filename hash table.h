#ifndef ASSEMBLER_HASH_TABLE_H
#define ASSEMBLER_HASH_TABLE_H
typedef struct {
    char* key;
    void* item;
}hash_item;
hash_item* create_hash_item(char* key, void* item);
void hash_insert(hash_item item);
hash_item* hash_item_alloc(int size);
#endif //ASSEMBLER_HASH_TABLE_H
