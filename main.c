#include <stdio.h>
#include <string.h>

#include "hash_table.h"

/* Mostra só as posições ocupadas ou apagadas, mais um resumo. */
static void ht_print(const ht_hash_table* ht, const char* titulo) {
    int ocupadas = 0, apagadas = 0;

    printf("\n=== %s ===\n", titulo);
    printf("size=%d  count=%d\n", ht->size, ht->count);

    for (int i = 0; i < ht->size; i++) {
        const ht_item* it = ht->items[i];
        if (it == NULL) continue;                 /* posição vazia: não imprime */

        if (it->key == NULL) {                    /* marcador de apagado */
            printf("  [%2d] <APAGADO>\n", i);
            apagadas++;
        } else {
            printf("  [%2d] %-8s -> %s\n", i, it->key, it->value);
            ocupadas++;
        }
    }
    printf("ocupadas=%d  apagadas=%d  vazias=%d\n",
           ocupadas, apagadas, ht->size - ocupadas - apagadas);
}

static void pesquisa(ht_hash_table* ht, const char* chave) {
    char* v = ht_search(ht, chave);
    printf("  search(\"%s\") = %s\n", chave, v ? v : "(nao existe)");
}

int main(void) {
    ht_hash_table* ht = ht_new();

    /* 1. Inserções */
    ht_insert(ht, "ana",   "912345678");
    ht_insert(ht, "rui",   "923456789");
    ht_insert(ht, "eva",   "956789012");
    ht_insert(ht, "leo",   "945678901");
    ht_insert(ht, "bia",   "934567890");
    ht_insert(ht, "pedro", "967890123");
    ht_insert(ht, "marta", "978901234");
    ht_insert(ht, "tiago", "989012345");
    ht_print(ht, "1. Depois de inserir 8 itens");

    /* 2. Atualização de chave existente */
    ht_insert(ht, "ana", "999999999");
    ht_print(ht, "2. Depois de atualizar 'ana' (count nao deve mudar)");
    pesquisa(ht, "ana");

    /* 3. Remoções */
    ht_delete(ht, "rui");
    ht_delete(ht, "leo");
    ht_delete(ht, "naoexiste");                   /* nao deve rebentar */
    ht_print(ht, "3. Depois de apagar 'rui' e 'leo'");
    pesquisa(ht, "rui");
    pesquisa(ht, "eva");                          /* deve continuar a ser encontrada */

    /* 4. Reinserção depois de apagar */
    ht_insert(ht, "rui", "900000000");
    ht_insert(ht, "sara", "911111111");
    ht_print(ht, "4. Depois de reinserir 'rui' e inserir 'sara'");
    pesquisa(ht, "rui");
    pesquisa(ht, "sara");

    /* 5. Libertar tudo */
    ht_del_hash_table(ht);
    ht = NULL;
    puts("\nTabela libertada.");
    return 0;
}