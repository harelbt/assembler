#include <stdlib.h>
#include "hash table.h"
#include "helpfunctions.h"
hash_item* create_hash_item(char* key, void* item){
hash_item* new_hash = hash_array_alloc(1);
new_hash->key = key;
new_hash->item = item;
return new_hash;
}
hash_item* hash_array_alloc(int size){
    hash_item* hashp = (hash_item*)calloc(size,sizeof(hash_item));
    if (hashp == NULL)
        stop(MEMORY,"");
    return hashp;
}