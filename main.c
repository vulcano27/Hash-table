//Ficheiro para testar a hash table
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "hash_table.h"

#ifndef N_ITEMS
#define N_ITEMS 40   /* abaixo de 53 posições; aumenta só se o resize já estiver implementado */
#endif

static void test_basico(void) {
    ht_hash_table* ht = ht_new();

    ht_insert(ht, "ana", "912345678");
    ht_insert(ht, "rui", "923456789");
    ht_insert(ht, "eva", "956789012");

    assert(strcmp(ht_search(ht, "ana"), "912345678") == 0);
    assert(strcmp(ht_search(ht, "rui"), "923456789") == 0);
    assert(strcmp(ht_search(ht, "eva"), "956789012") == 0);
    assert(ht_search(ht, "bia") == NULL);

    ht_del_hash_table(ht);
    puts("test_basico OK");
}

static void test_atualizar(void) {
    ht_hash_table* ht = ht_new();

    ht_insert(ht, "ana", "111");
    ht_insert(ht, "ana", "222");   /* mesma chave: deve substituir */

    assert(strcmp(ht_search(ht, "ana"), "222") == 0);

    ht_del_hash_table(ht);
    puts("test_atualizar OK");
}

static void test_apagar(void) {
    ht_hash_table* ht = ht_new();

    ht_insert(ht, "ana", "1");
    ht_insert(ht, "rui", "2");
    ht_insert(ht, "eva", "3");

    ht_delete(ht, "rui");
    assert(ht_search(ht, "rui") == NULL);
    assert(strcmp(ht_search(ht, "ana"), "1") == 0);   /* cadeias não podem partir */
    assert(strcmp(ht_search(ht, "eva"), "3") == 0);

    ht_delete(ht, "naoexiste");                        /* não deve rebentar */

    ht_insert(ht, "rui", "novo");                      /* reinserir depois de apagar */
    assert(strcmp(ht_search(ht, "rui"), "novo") == 0);

    ht_del_hash_table(ht);
    puts("test_apagar OK");
}

static void test_muitos(void) {
    ht_hash_table* ht = ht_new();
    char key[32], val[32];

    for (int i = 0; i < N_ITEMS; i++) {
        snprintf(key, sizeof key, "chave%d", i);
        snprintf(val, sizeof val, "valor%d", i);
        ht_insert(ht, key, val);
    }
    for (int i = 0; i < N_ITEMS; i++) {
        snprintf(key, sizeof key, "chave%d", i);
        snprintf(val, sizeof val, "valor%d", i);
        char* r = ht_search(ht, key);
        assert(r != NULL && strcmp(r, val) == 0);
    }
    for (int i = 0; i < N_ITEMS; i += 2) {             /* apaga metade */
        snprintf(key, sizeof key, "chave%d", i);
        ht_delete(ht, key);
    }
    for (int i = 0; i < N_ITEMS; i++) {
        snprintf(key, sizeof key, "chave%d", i);
        if (i % 2 == 0) assert(ht_search(ht, key) == NULL);
        else            assert(ht_search(ht, key) != NULL);
    }

    ht_del_hash_table(ht);
    puts("test_muitos OK");
}

int main(void) {
    test_basico();
    test_atualizar();
    test_apagar();
    test_muitos();
    puts("Todos os testes passaram.");
    return 0;
}