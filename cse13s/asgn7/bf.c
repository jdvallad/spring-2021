#include "bf.h"

#include "bv.c"
#include "speck.c"

#include <stdio.h>

struct BloomFilter {
    uint64_t primary[2];
    uint64_t secondary[2];
    uint64_t tertiary[2];
    BitVector *filter;
};

// Constructor Function for a BloomFilter. Instantiates the 3 salts that are
// used for hashing, and creates the underlying BitVector. Returns resulting
// BloomFilter.
BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    if (bf) {
        // Grimm's Fairy Tales
        bf->primary[0] = 0x5adf08ae86d36f21;
        bf->primary[1] = 0xa267bbd3116f3957;
        // The Adventures of Sherlock Holmes
        bf->secondary[0] = 0x419d292ea2ffd49e;
        bf->secondary[1] = 0x09601433057d5786;
        // The Strange Case of Dr. Jekyll and Mr. Hyde
        bf->tertiary[0] = 0x50d8bb08de3818df;
        bf->tertiary[1] = 0x4deaae187c16ae1d;
        bf->filter = bv_create(size);
        if (!bf->filter) {
            free(bf);
            bf = NULL;
        }
    }
    return bf;
}

// Destructor Method for a BloomFilter. Frees the underlying BitVector, and then
// frees the memory for the BloomFilter bf. Finally, pointer to bf is set to
// NULL.
void bf_delete(BloomFilter **bf) {
    if (*bf) {
        bv_delete(&(*bf)->filter);
        free(*bf);
        *bf = NULL;
    }
    return;
}

// Returns the size of the BloomFilter.
uint32_t bf_size(BloomFilter *bf) {
    return bv_length(bf->filter);
}

// Inserts oldspeak into the BloomFilter by setting the 3 bits associated with
// its 3 different hashes.
void bf_insert(BloomFilter *bf, char *oldspeak) {
    bv_set_bit(bf->filter, hash(bf->primary, oldspeak) % bf_size(bf));
    bv_set_bit(bf->filter, hash(bf->secondary, oldspeak) % bf_size(bf));
    bv_set_bit(bf->filter, hash(bf->tertiary, oldspeak) % bf_size(bf));
    return;
}

// Returns true if oldspeak MAY be in the BloomFilter, and false if it certainly
// is not in the BloomFilter.
bool bf_probe(BloomFilter *bf, char *oldspeak) {
    return bv_get_bit(bf->filter, hash(bf->primary, oldspeak) % bf_size(bf))
           && bv_get_bit(bf->filter, hash(bf->secondary, oldspeak) % bf_size(bf))
           && bv_get_bit(bf->filter, hash(bf->tertiary, oldspeak) % bf_size(bf));
}

// Returns the number of bits in the underlying BitVector that have been set.
uint32_t bf_count(BloomFilter *bf) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < bf_size(bf); i++) {
        if (bv_get_bit(bf->filter, i)) {
            count++;
        }
    }
    return count;
}

// Helper Function that prints the underlying BitVector to the screen.
void bf_print(BloomFilter *bf) {
    bv_print(bf->filter);
    return;
}
