#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

#include "hash_table.h"
#include "prime.h"

#define HT_PRIME_1 151
#define HT_PRIME_2 163
#define HT_INITIAL_BASE_SIZE 50

static ht_item APAGADO = {NULL, NULL};


static ht_item* ht_new_item(const char* k, const char* v){
    ht_item* i = malloc(sizeof(ht_item));
    i->key = strdup(k);
    i->value = strdup(v);
    return i;
}

ht_hash_table* ht_new() {
    
    return ht_new_sized(HT_INITIAL_BASE_SIZE);
}

static ht_hash_table* ht_new_sized(const int base_size){
    ht_hash_table* ht = xmalloc(sizeof(ht_hash_table));
    ht->base_size = base_size;
    ht->size = next_prime(base_size);
    ht->count = 0;
    ht->items = xcalloc((size_t)ht->size, sizeof(ht_item*));
    return ht;
}

static void ht_resize(ht_hash_table* ht, const int base_size){
    if(base_size < HT_INITIAL_BASE_SIZE) return;
    ht_hash_table* new_ht = ht_new();
    for(int i; i < ht->size; i++)
        if(ht->items[i] != NULL && ht->items[i] != &APAGADO)
            ht_insert(new_ht, ht->items[i]->key, ht->items[i]->value);
    
    ht->base_size = new_ht->base_size;
    ht->count = new_ht->count;
    
    const int tmp_size = ht->size;
    ht->size = new_ht->size;
    new_ht->size = tmp_size;

    const int tmp_items = ht->items; //ponteiro para a array de ponteiros de itens
    ht->items = new_ht->items;
    new_ht->items = tmp_items;

    ht_del_hash_table(new_ht);
}

static void ht_del_item(ht_item* i) {
    free(i->key);
    free(i->value);
    free(i);
}


void ht_del_hash_table(ht_hash_table* ht) {
    for (int i = 0; i < ht->size; i++)
        if (ht->items[i] != NULL) 
            ht_del_item(ht->items[i]);       
    free(ht->items);
    free(ht);
}

static int ht_hash(const char* s, const int a, const int m) {
    long hash = 0;
    const int len_s = strlen(s);
    for (int i = 0; i < len_s; i++) {
        hash += (long)pow(a, len_s - (i+1)) * s[i];
        hash = hash % m;
    }
    return (int)hash;
}

static int ht_get_hash(const char* s, const int num_buckets, const int attempt) {
    const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
    const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

void ht_insert(ht_hash_table* ht, const char* key, const char* value){
    ht_item* item = ht_new_item(key, value);
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item_atual = ht->items[index];
    int attempt = 1;
    while(item != NULL){
        if(item != &APAGADO && strcmp(key, item->key) == 0){
            ht_del_item(item_atual);
            ht->items[index] = item;
            return;
        }
        item_atual = ht_get_hash(key, ht->size, attempt);
        attempt++;
    }
    ht->items[index] = item;
    ht->count++;
}

char* ht_search(ht_hash_table* ht, const char* key) {
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int attempt = 1;
    while(item != NULL){
        if(item != &APAGADO && strcmp(key, item->key) == 0) return item->value;
        index = ht_get_hash(key, ht->size, attempt);
        item = ht->items[index];
        attempt++;
    }
    return NULL;
}

void ht_delete(ht_hash_table* ht, const char* key){
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int attemp = 1;
    while(item != NULL){
        if(item != &APAGADO && strcmp(key, item->key) == 0){
            ht_del_item(ht->items[index]);
            ht->items[index] = &APAGADO;
        }
        index = ht_get_hash(key, ht->size, attemp);
        item = ht->items[index];
        attemp++;
    }
    ht->count--;
}