#include "stack.h"

#include "node.c"

#include <stdlib.h>
struct Stack {
    uint32_t top;
    uint32_t capacity;
    Node **items;
};
//Provided in Assignment 3 Instructions PDF
Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    if (s) {
        s->top = 0;
        s->capacity = capacity;
        s->items = (Node **) calloc(capacity, sizeof(Node *));
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
        for (uint32_t i = 0; i < stack_size(*s); i++) {
            free((*s)->items[i]);
        }
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

// Adds node to stack if there is room on the stack. Returns true if operation was successful.
bool stack_push(Stack *s, Node *n) {
    if (stack_full(s)) {
        return false;
    }
    s->items[s->top] = n;
    s->top++;
    return true;
}

// Removes node from stack if size is positive. Returns true if operation was successful.
bool stack_pop(Stack *s, Node **n) {
    if (stack_empty(s)) {
        return false;
    }
    s->top--;
    *n = s->items[s->top];
    return true;
}
// "Prints out the contents of the stack to outfile using printf().
void stack_print(Stack *s) {
    for (uint32_t i = 0; i < s->top; i += 1) {
        node_print(s->items[i]);
    }
    printf("\n");
    return;
}
