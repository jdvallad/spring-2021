#include "queue.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
struct Queue {
    uint32_t head;
    uint32_t tail;
    uint32_t size;
    uint32_t capacity;
    int64_t *items;
};
//Given in PDF
Queue *queue_create(uint32_t capacity) {
    Queue *q = (Queue *) malloc(sizeof(Queue));
    if (q) {
        q->head = 0;
        q->tail = 0;
        q->size = 0;
        q->capacity = capacity;
        q->items = (int64_t *) calloc(capacity, sizeof(int64_t));
        if (!q->items) {
            free(q);
            q = NULL;
        }
    }
    return q;
}
//Given in PDF
void queue_delete(Queue **q) {
    if (*q && (*q)->items) {
        free((*q)->items);
        free(*q);
        *q = NULL;
    }
    return;
}

bool queue_empty(Queue *q) {
    return q->size == 0;
}

bool queue_full(Queue *q) {
    return q->size == q->capacity;
}

uint32_t queue_size(Queue *q) {
    return q->size;
}
// tail is updated based on current location.
// If tail overflows past end of memory, loops back around to beginning of allocated memory.
bool enqueue(Queue *q, int64_t x) {
    if (queue_full(q)) {
        return false;
    }
    q->items[q->tail] = x;
    q->tail = (q->tail + 1) % q->capacity;
    q->size++;
    return true;
}

// head is updated based on current location.
// If head overflow past beginning of memory, loops back aroud to end of allocated memory.
bool dequeue(Queue *q, int64_t *x) {
    if (queue_empty(q)) {
        return false;
    }
    *x = q->items[q->head];
    q->head = (q->head + 1) % q->capacity;
    q->size--;
    return true;
}

void queue_print(Queue *q) {
    if (q->size == 0) {
        printf("[]\n");
        return;
    }
    printf("[%ld", q->items[q->head]);
    for (uint32_t i = (1 + q->head) % q->capacity; i != q->tail; i = (i + 1) % q->capacity) {
        printf(", %ld", q->items[i]);
    }
    printf("]\n");
    return;
}
