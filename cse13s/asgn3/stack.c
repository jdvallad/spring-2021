#include "stack.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
struct Stack {
    uint32_t top;
    uint32_t capacity;
    int64_t *items;
};
//Provided in Assigment PDF
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
//Provided in Assigment PDF
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

bool stack_push(Stack *s, int64_t x) {
    if (stack_full(s)) {
        return false;
    }
    s->items[s->top] = x;
    s->top++;
    return true;
}

bool stack_pop(Stack *s, int64_t *x) {
    if (stack_empty(s)) {
        return false;
    }
    s->top--;
    *x = s->items[s->top];
    return true;
}

void stack_print(Stack *s) {
    if (s->top == 0) {
        printf("[]\n");
        return;
    }
    printf("[%ld", s->items[0]);
    for (uint32_t i = 1; i < s->top; i++) {
        printf(", %ld", s->items[i]);
    }
    printf("]\n");
    return;
}
