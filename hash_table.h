#ifndef HASH_TABLE
#define HASH_TABLE
typedef struct {
    char* key;
    char* value;
} ht_item;

typedef struct {
    int size;
    int base_size;
    int count;
    ht_item** items;
} ht_hash_table;


//Criação da hash table

ht_hash_table* ht_new();

//Libertação de memória

void ht_del_hash_table(ht_hash_table* ht);

//Métodos

void ht_insert(ht_hash_table* ht, const char* key, const char* value);

char* ht_search(ht_hash_table* ht, const char* key);

void ht_delete(ht_hash_table* h, const char* key);

#endif