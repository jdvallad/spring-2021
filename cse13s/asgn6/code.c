#include "code.h"

#include "defines.h"

#include <inttypes.h>
#include <stdio.h>

// Constructor for Code
// allocates memory to the stack, thus no destructor function
// initial size is 0, all bytes are 0
Code code_init(void) {
    struct Code c;
    c.top = 0;
    for (int i = 0; i < MAX_CODE_SIZE; i++) {
        c.bits[i] = 0;
    }
    return c;
}
// returns size of the Code
uint32_t code_size(Code *c) {
    return c->top;
}

bool code_empty(Code *c) {
    return code_size(c) == 0;
}

bool code_full(Code *c) {
    return code_size(c) == ALPHABET;
}

// Sets the ith bit of Code c to 1
void code_set_bit(Code *c, uint32_t i) {
    c->bits[i / 8] |= (1 << (i % 8));
    return;
}

// Sets the ith bit of Code c to 0
void code_clr_bit(Code *c, uint32_t i) {
    c->bits[i / 8] &= (~(1 << (i % 8)));
    return;
}

// returns the ith bit of Code c
uint8_t code_get_bit(Code *c, uint32_t i) {
    return 1 & (c->bits[i / 8] >> (i % 8));
}

// Stack like behavior, pushes bit onto top of Code c if it isn't full and increments size
// returns whether action was successful
bool code_push_bit(Code *c, uint8_t bit) {
    if (code_full(c)) {
        return false;
    }
    if (bit) {
        code_set_bit(c, c->top);
    } else {
        code_clr_bit(c, c->top);
    }
    c->top++;
    return true;
}
// Stack like behavior, pops bit of top of Code c if it isn't empty and decrements size
// returns whether action was successful
bool code_pop_bit(Code *c, uint8_t *bit) {
    if (code_empty(c)) {
        return false;
    }
    c->top--;
    *bit = code_get_bit(c, c->top);
    code_clr_bit(c, c->top);
    return true;
}

// Helper method that prints the code to the screen.
void code_print(Code *c) {
    printf("[");
    for (uint32_t i = 0; i < code_size(c); i++) {
        printf("%1" PRIu8, code_get_bit(c, i));
    }
    printf("]\n");
    return;
}
