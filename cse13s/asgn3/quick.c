#include "quick.h"

#include "queue.c"
#include "stack.c"

#include <inttypes.h>
#include <stdio.h>

extern long max_stack;
extern long max_queue;
extern long moves;
extern long compares;
int64_t partition(uint32_t *, int64_t, int64_t);
void stack_test(void);
void queue_test(void);

// Pseudocode given in PDF
void quick_sort_stack(uint32_t *A, uint32_t n) {
    max_stack = 0;
    moves = 0;
    compares = 0;
    int64_t lo = 0;
    int64_t hi = n - 1;
    int64_t p = 0;
    Stack *stack = stack_create(n);
    stack_push(stack, lo);
    stack_push(stack, hi);
    if (stack_size(stack) > max_stack) {
        max_stack = stack_size(stack);
    }
    while (stack_size(stack) != 0) {
        stack_pop(stack, &hi);
        stack_pop(stack, &lo);
        p = partition(A, lo, hi);
        if (lo < p) {
            stack_push(stack, lo);
            stack_push(stack, p);
            if (stack_size(stack) > max_stack) {
                max_stack = stack_size(stack);
            }
        }
        if (hi > p + 1) {
            stack_push(stack, p + 1);
            stack_push(stack, hi);
            if (stack_size(stack) > max_stack) {
                max_stack = stack_size(stack);
            }
        }
    }
    stack_delete(&stack);
    return;
}

// Pseudocode given in PDF.
void quick_sort_queue(uint32_t *A, uint32_t n) {
    max_queue = 0;
    moves = 0;
    compares = 0;
    int64_t lo = 0;
    int64_t hi = n - 1;
    int64_t p = 0;
    Queue *queue = queue_create(n);
    enqueue(queue, lo);
    enqueue(queue, hi);
    if (queue_size(queue) > max_queue) {
        max_queue = queue_size(queue);
    }
    while (queue_size(queue) != 0) {
        dequeue(queue, &lo);
        dequeue(queue, &hi);
        p = partition(A, lo, hi);
        if (lo < p) {
            enqueue(queue, lo);
            enqueue(queue, p);
            if (queue_size(queue) > max_queue) {
                max_queue = queue_size(queue);
            }
        }
        if (hi > p + 1) {
            enqueue(queue, p + 1);
            enqueue(queue, hi);
            if (queue_size(queue) > max_queue) {
                max_queue = queue_size(queue);
            }
        }
    }
    queue_delete(&queue);
    return;
}

// Pseudocode given in PDF, save for small change.
// Change was added as pseudocode as given in PDF caused segmentation faults in my code.
// I am not completely sure why.
int64_t partition(uint32_t *A, int64_t lo, int64_t hi) {
    uint32_t pivot = A[lo + ((hi - lo) / 2)];
    uint64_t i = lo; //originally lo - 1
    uint64_t j = hi; // originally hi + 1
    uint64_t temp = 0;
    while (i < j) {
        //originally i++;
        while (A[i] < pivot) {
            i++;
            compares++;
        }
        //originally j--;
        while (A[j] > pivot) {
            j--;
            compares++;
        }
        if (i < j) {
            temp = A[i];
            A[i] = A[j];
            A[j] = temp;
            moves += 3;
        }
    }
    return j;
}

// Test method used when implementing stack
void stack_test() {
    int64_t temp = 0;
    Stack *s = stack_create(10);
    stack_print(s);
    for (int j = 0; j < 2; j++) {
        for (int i = 0; !stack_full(s); i++) {
            stack_push(s, i);
            stack_print(s);
        }
        while (!stack_empty(s)) {
            stack_pop(s, &temp);
            printf("%ld, ", temp);
            stack_print(s);
        }
    }
}
// Test method used when implementing queue
void queue_test() {
    int64_t temp = 0;
    Queue *q = queue_create(10);
    queue_print(q);
    for (int j = 0; j < 2; j++) {
        for (int i = 0; !queue_full(q); i++) {
            enqueue(q, i);
            queue_print(q);
        }
        while (!queue_empty(q)) {
            dequeue(q, &temp);
            printf("%ld, ", temp);
            queue_print(q);
        }
    }
}
