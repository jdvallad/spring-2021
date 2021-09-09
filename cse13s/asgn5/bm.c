#include "bm.h"

#include "bv.c"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

struct BitMatrix {
    uint32_t rows;
    uint32_t cols;
    BitVector *vector;
};

// BitMatrix Constructor
BitMatrix *bm_create(uint32_t rows, uint32_t cols) {
    BitMatrix *m = (BitMatrix *) malloc(sizeof(BitMatrix));
    if (m) {
        m->rows = rows;
        m->cols = cols;
        m->vector = bv_create(rows * cols);
        if (!m->vector) {
            free(m);
            m = NULL;
        }
    }
    return m;
}

// BitMatrix Destructor
void bm_delete(BitMatrix **m) {
    if (*m) {
        if ((*m)->vector) {
            bv_delete(&((*m)->vector));
        }
        free(*m);
        *m = NULL;
    }
    return;
}

uint32_t bm_rows(BitMatrix *m) {
    return m->rows;
}

uint32_t bm_cols(BitMatrix *m) {
    return m->cols;
}

void bm_set_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    bv_set_bit(m->vector, r * (m->cols) + c);
}

void bm_clr_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    bv_clr_bit(m->vector, r * (m->cols) + c);
}

uint8_t bm_get_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    return bv_get_bit(m->vector, r * (m->cols) + c);
}

// returns the first length bits from byte as a 1 x length BitMatrix
// 0 < length <= 8
BitMatrix *bm_from_data(uint8_t byte, uint32_t length) {
    BitMatrix *m = bm_create(1, length > 8 ? 8 : length);
    (&(m->vector[0]))->vector[0] = byte;
    return m;
}

// converts the first 8 bits of BitMatrix m into a uint8_t
uint8_t bm_to_data(BitMatrix *m) {
    return (&(m->vector[0]))->vector[0];
}

// performs a standard matrix multiplication operation and returns the result
// resulting matrix is mod 2 (all elements are 0 or 1)
BitMatrix *bm_multiply(BitMatrix *A, BitMatrix *B) {
    uint32_t m = bm_rows(A), n = bm_cols(A), p = bm_cols(B);
    BitMatrix *C = bm_create(m, p);
    for (uint32_t i = 0; i < m; i++) {
        for (uint32_t j = 0; j < p; j++) {
            bm_clr_bit(C, i, j);
            uint8_t sum = 0;
            for (uint32_t k = 0; k < n; k++) {
                sum += bm_get_bit(A, i, k) * bm_get_bit(B, k, j);
            }
            sum %= 2;
            if (sum) {
                bm_set_bit(C, i, j);
            }
        }
    }
    return C;
}

// Helper print method that will print the bit matrix to the screen in a grid pattern
void bm_print(BitMatrix *m) {
    printf("\n");
    for (uint32_t r = 0; r < bm_rows(m); r++) {
        printf("  ");
        for (uint32_t c = 0; c < bm_cols(m); c++) {
            printf("%1" PRIu8 " ", bm_get_bit(m, r, c));
        }
        printf("\n");
    }
    printf("\n");
    return;
}

// fills the input BitMatrix with elements from input string array.
// each string should be made up of only '0' and '1' characters given in the order desired in each
// row. Each row given should be the same length
void bm_fill(BitMatrix *m, char *arr[]) {
    for (uint32_t r = 0; r < bm_rows(m); r++) {
        for (uint32_t c = 0; c < bm_cols(m); c++) {
            bm_clr_bit(m, r, c);
            if (arr[r][c] - '0') {
                bm_set_bit(m, r, c);
            }
        }
    }
}

// Speciality BitMatrix constructor that creates a BitMatrix from an input string array.
// See bm_fill.
BitMatrix *bm_create_fill(uint32_t rows, uint32_t cols, char *arr[]) {
    BitMatrix *m = bm_create(rows, cols);
    bm_fill(m, arr);
    return m;
}
