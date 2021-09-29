#include "ns.h"

#include <stdlib.h>

struct ns {
    uint32_t top;
    uint32_t capacity;
    uint32_t *items;
};
//Provided in Assignment 3 Instructions PDF
ns *ns_create(uint32_t capacity) {
    ns *s = (ns *) malloc(sizeof(ns));
    if (s) {
        s->top = 0;
        s->capacity = capacity;
        s->items = (uint32_t *) calloc(capacity, sizeof(uint32_t));
        if (!s->items) {
            free(s);
            s = NULL;
        }
    }
    return s;
}
//Provided in Assignment 3 Instructions PDF
void ns_delete(ns **s) {
    if (*s && (*s)->items) {
        free((*s)->items);
        free(*s);
        *s = NULL;
    }
    return;
}

bool ns_empty(ns *s) {
    return s->top == 0;
}

bool ns_full(ns *s) {
    return s->top == s->capacity;
}

uint32_t ns_size(ns *s) {
    return s->top;
}

// Adds element to ns if there is room on the ns. Returns true if operation was successful.
bool ns_push(ns *s, uint32_t x) {
    if (ns_full(s)) {
        return false;
    }
    s->items[s->top] = x;
    s->top++;
    return true;
}

// Removes element from ns if size is positive. Returns true if operation was successful.
bool ns_pop(ns *s, uint32_t *x) {
    if (ns_empty(s)) {
        return false;
    }
    s->top--;
    *x = s->items[s->top];
    return true;
}

// Stores value at the top of the ns in x. Returns true if operation was successful.
// Does so by popping from the ns and then incrementing top pointer if operation was successful.
bool ns_peek(ns *s, uint32_t *x) {
    bool res = ns_pop(s, x);
    if (res) {
        s->top++;
    }
    return res;
}

// Copies ns from src to dst. Assumption is both nss are properly initialized.
void ns_copy(ns *dst, ns *src) {
    if (dst->capacity != src->capacity) {
        return;
    }
    dst->top = src->top;
    for (uint32_t i = 0; i < dst->top; i++) {
        dst->items[i] = src->items[i];
    }
    return;
}

// "Prints out the contents of the ns to outfile using printf().
void ns_print(ns *s) {
    if (ns_empty(s)) {
        printf("[]\n");
        return;
    }
    printf("[%" PRIu32, s->items[0]);
    for (uint32_t i = 1; i < s->top; i += 1) {
        printf(", %" PRIu32, s->items[i]);
    }
    printf("]\n");
    return;
}
