#include "bv.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

// BitVector Constructor
BitVector *bv_create(uint32_t length) {
    BitVector *v = (BitVector *) malloc(sizeof(BitVector));
    if (v) {
        v->length = length;
        v->vector = (uint8_t *) calloc(1 + ((length - 1) / 8), sizeof(uint8_t));
        if (!v->vector) {
            free(v);
            v = NULL;
        }
    }
    return v;
}

// BitVector Destructor
void bv_delete(BitVector **v) {
    if (*v) {
        if ((*v)->vector) {
            free((*v)->vector);
        }
        free(*v);
        *v = NULL;
    }
    return;
}

uint32_t bv_length(BitVector *v) {
    return v->length;
}

// sets the ith bit to a 1
void bv_set_bit(BitVector *v, uint32_t i) {
    uint8_t bit = 1 << (i % 8);
    v->vector[i / 8] |= bit;
    return;
}

// sets the ith bit to a 0
void bv_clr_bit(BitVector *v, uint32_t i) {
    uint8_t bit = 1 << (i % 8);
    bit = ~bit;
    v->vector[i / 8] &= bit;
    return;
}

// returns the ith bit
uint8_t bv_get_bit(BitVector *v, uint32_t i) {
    return 1 & (v->vector[i / 8] >> (i % 8));
}

// xors the ith bit with the bit variable
void bv_xor_bit(BitVector *v, uint32_t i, uint8_t bit) {
    uint8_t get = bv_get_bit(v, i);
    get ^= bit;
    if (get) {
        bv_set_bit(v, i);
    } else {
        bv_clr_bit(v, i);
    }
    return;
}
// helper method that prints a BitVector to stdout
void bv_print(BitVector *v) {
    for (uint32_t i = 0; i < v->length; i++) {
        printf("%1" PRIu8, bv_get_bit(v, i));
    }
    printf("\n");
    return;
}
