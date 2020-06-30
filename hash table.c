#include <stdlib.h>
#include "hash table.h"
hash_item* create_hash_item(char* key, void* item){
hash_item* new_hash = (hash_item*)calloc(1,sizeof(hash_item));
new_hash->key = key;
new_hash->item = item;
    return new_hash;
}