#include "hamming.h"

#include "nibble.c"

// lookup table for error syndromes
// Positive entries signify which bit was flipped erroneously
// HAM_OK means no bit was flipped
// HAM_ERR means that more than one bit was flipped and thus the message cannot be corrected
int8_t lookup[] = { HAM_OK, 4, 5, HAM_ERR, 6, HAM_ERR, HAM_ERR, 3, 7, HAM_ERR, HAM_ERR, 2, HAM_ERR,
    1, 0, HAM_ERR };

// encodes lower nibble of msg using Hamming(8,4) encoding to a full byte
uint8_t ham_encode(BitMatrix *G, uint8_t msg) {
    BitMatrix *m = bm_from_data(msg, 4);
    BitMatrix *c = bm_multiply(m, G);
    uint8_t res = bm_to_data(c);
    bm_delete(&m);
    bm_delete(&c);
    return res;
}

// decodes a byte using Hamming(8,4) into a nibble of data. This decoded nibble is then stored in
// the lower nibble of *msg
// An int is then returned corresponding to the result of the decoding.
// HAM_OK means there were no errors
// HAM_CORRECT means that an error was present and corrected
// HAM_ERR means that more than one error was present and thus is uncorrectable
HAM_STATUS ham_decode(BitMatrix *Ht, uint8_t code, uint8_t *msg) {
    BitMatrix *c = bm_from_data(code, 8);
    BitMatrix *e = bm_multiply(c, Ht);
    *msg = pack_byte(0, upper_nibble(code));
    uint8_t error = bm_to_data(e);
    bm_delete(&c);
    bm_delete(&e);
    int8_t status = lookup[error];
    if (status == HAM_OK) {
        return HAM_OK;
    }
    if (status == HAM_ERR) {
        return HAM_ERR;
    }
    *msg ^= (0x1 << (status - 4));
    *msg = lower_nibble(*msg);
    return HAM_CORRECT;
}
