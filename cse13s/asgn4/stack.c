#include "stack.h"

#include <stdlib.h>
struct Stack {
    uint32_t top;
    uint32_t capacity;
    int64_t *items;
};
//Provided in Assignment 3 Instructions PDF
Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    if (s) {
        s->top = 0;
        s->capacity = capacity;
        s->items = (int64_t *) calloc(capacity, sizeof(int64_t));
        if (!s->items) {
            free(s);
            s = NULL;
        }
    }
    return s;
}
//Provided in Assignment 3 Instructions PDF
void stack_delete(Stack **s) {
    if (*s && (*s)->items) {
        free((*s)->items);
        free(*s);
        *s = NULL;
    }
    return;
}

bool stack_empty(Stack *s) {
    return s->top == 0;
}

bool stack_full(Stack *s) {
    return s->top == s->capacity;
}

uint32_t stack_size(Stack *s) {
    return s->top;
}

// Adds element to stack if there is room on the stack. Returns true if operation was successful.
bool stack_push(Stack *s, uint32_t x) {
    if (stack_full(s)) {
        return false;
    }
    s->items[s->top] = x;
    s->top++;
    return true;
}

// Removes element from stack if size is positive. Returns true if operation was successful.
bool stack_pop(Stack *s, uint32_t *x) {
    if (stack_empty(s)) {
        return false;
    }
    s->top--;
    *x = s->items[s->top];
    return true;
}

// Stores value at the top of the stack in x. Returns true if operation was successful.
// Does so by popping from the stack and then incrementing top pointer if operation was successful.
bool stack_peek(Stack *s, uint32_t *x) {
    bool res = stack_pop(s, x);
    if (res) {
        s->top++;
    }
    return res;
}

// Copies stack from src to dst. Assumption is both stacks are properly initialized.
void stack_copy(Stack *dst, Stack *src) {
    if (dst->capacity != src->capacity) {
        return;
    }
    dst->top = src->top;
    for (uint32_t i = 0; i < dst->top; i++) {
        dst->items[i] = src->items[i];
    }
    return;
}

// "Prints out the contents of the stack to outfile using fprintf() . Working through each vertex
// in the stack starting from the bottom, print out the name of the city each vertex corresponds to."
// Description and code given in Assigment 4 PDF.
void stack_print(Stack *s, FILE *outfile, char *cities[]) {
    fprintf(outfile, "Path: ");
    for (uint32_t i = 0; i < s->top; i += 1) {
        fprintf(outfile, "%s", cities[s->items[i]]);
        if (i + 1 != s->top) {
            fprintf(outfile, " -> ");
        }
    }
    fprintf(outfile, "\n");
}
