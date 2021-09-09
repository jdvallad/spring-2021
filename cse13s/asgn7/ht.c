#include "ht.h"

#include "ll.c"

#include <stdio.h>

struct HashTable {
    uint64_t salt[2];
    uint64_t size;
    bool mtf;
    LinkedList **lists;
};

// Constructor Function for a HashTable
// Dynamically Allocates memory for both the HashTable and the underlying
// LinkedList
HashTable *ht_create(uint32_t size, bool mtf) {
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    if (ht) {
        // Leviathan
        ht->salt[0] = 0x9846e4f157fe8840;
        ht->salt[1] = 0xc5f318d7e055afb8;
        ht->size = size;
        ht->mtf = mtf;
        ht->lists = (LinkedList **) calloc(size, sizeof(LinkedList *));
        if (!ht->lists) {
            free(ht);
            ht = NULL;
        }
    }
    return ht;
}

// Destructor Method for a HashTable
// frees the memory for the HashTable's underlying LinkedList, then frees the
// memory for the HashTable itself. The pointer for the HashTable is then set to
// NULL.
void ht_delete(HashTable **ht) {
    if ((*ht) && (*ht)->lists) {
        for (uint32_t i = 0; i < (*ht)->size; i++) {
            if ((*ht)->lists[i]) {
                ll_delete(&(*ht)->lists[i]);
            }
        }
        free((*ht)->lists);
        free(*ht);
        *ht = NULL;
    }
    return;
}

// Returns the size of the HashTable
uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

// Looks for the existence of a node within the HashTable that has an oldspeak
// field equal to the parameter oldspeak. If it does, it returns that node.
// If it doesn't this returns NULL.
Node *ht_lookup(HashTable *ht, char *oldspeak) {
    LinkedList *ll = ht->lists[hash(ht->salt, oldspeak) % ht->size];
    if (ll) {
        return ll_lookup(ll, oldspeak);
    }
    return NULL;
}

// Inserts a node with the input oldspeak and newspeak fields into the
// HashTable. If the underlying LinkedList for the new node has not been
// instantiated, it is instantiated. Return void.
void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    uint32_t ind = hash(ht->salt, oldspeak) % ht->size;
    if (ht->lists[ind]) {
        ll_insert(ht->lists[ind], oldspeak, newspeak);
        return;
    }
    ht->lists[ind] = ll_create(ht->mtf);
    ll_insert(ht->lists[ind], oldspeak, newspeak);
    return;
}

// Returns the number of LinkedLists in the HashTable that have been
// instantiated.
uint32_t ht_count(HashTable *ht) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < ht->size; i++) {
        if (ht->lists[i]) {
            count++;
        }
    }
    return count;
}

// Helper method that prints the HashTable to the screen.
void ht_print(HashTable *ht) {
    for (uint32_t i = 0; i < ht->size; i++) {
        if (ht->lists[i]) {
            ll_print(ht->lists[i]);
        }
    }
    return;
}
