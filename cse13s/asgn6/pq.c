#include "pq.h"

#include "ns.c"

#include <stdio.h>
#define EMPTY 4294967295 // This is the maximum value for a uint32_t, reserved to signify that
// an index does not store a node.

struct PriorityQueue {
    uint32_t capacity;
    ns *available_indices;
    Node **nodes;
    uint32_t *indices;
};

// Constructor method for PriorityQueue.
// PriorityQueue consists of ar array of Node*, an array, of sorted indices, and a stack of free
// indices.
PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *pq = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    if (pq) {
        pq->capacity = capacity;
        pq->nodes = ((Node **) malloc(capacity * sizeof(Node *)));
        if (!pq->nodes) {
            free(pq);
            pq = NULL;
            return pq;
        }
        pq->indices = (uint32_t *) malloc(capacity * sizeof(uint32_t));
        if (!pq->indices) {
            free(pq->nodes);
            free(pq);
            pq = NULL;
            return pq;
        }
        pq->available_indices = ns_create(capacity);
        if (!pq->available_indices) {
            free(pq->indices);
            free(pq->nodes);
            free(pq);
            pq = NULL;
            return pq;
        }
        for (uint32_t i = 0; i < capacity; i++) {
            ns_push(pq->available_indices, i); // All indices are free
            pq->indices[i] = EMPTY; //  to signify that there is no node there
            pq->nodes[i] = NULL; // Not specifically necessary, initializing all nodes to NULL
        }
    }
    return pq;
}

// Destructor method for PriorityQueue
// IMPORTANT: This destructor method will free all nodes currently in the node array. This means
// that even if a node was popped, unless that node was overridden by pushing another node to that
// index, that node will be freed when the PriorityQueue is freed. This means you should most
// likely save the freeing of the PriorityQueue for after the freeing of all nodes.
//
// EDIT: This seems to be fixed.
void pq_delete(PriorityQueue **q) {
    for (uint32_t i = 0; i < (*q)->capacity; i++) {
        node_delete(&((*q)->nodes[i]));
    }
    free((*q)->nodes);
    free((*q)->indices);
    ns_delete(&((*q)->available_indices));
    free(*q);
    *q = NULL;
    return;
}

bool pq_empty(PriorityQueue *q) {
    return ns_full(q->available_indices);
}

bool pq_full(PriorityQueue *q) {
    return ns_empty(q->available_indices);
}

uint32_t pq_size(PriorityQueue *q) {
    return q->capacity - ns_size(q->available_indices);
}

// Enqueues node n into PriorityQueue q. Successful if q isn't full. Returns whether action was
// completed successfully.
bool enqueue(PriorityQueue *q, Node *n) {
    uint32_t size = pq_size(q);
    if (pq_full(q)) {
        return false;
    }
    uint32_t index;
    ns_pop(q->available_indices, &index);
    q->nodes[index] = n;
    uint32_t sorted_index = 0;
    while (q->indices[sorted_index] != EMPTY
           && q->nodes[index]->frequency > q->nodes[q->indices[sorted_index]]->frequency) {
        sorted_index++;
    }
    for (uint32_t i = size; i > sorted_index; i--) {
        q->indices[i] = q->indices[i - 1];
    }
    q->indices[sorted_index] = index;
    return true;
}

// Dequeues node from PriorityQueue q into pointer given by n. Successful if q isn't empty.
// Returns whether action was completed successfully.
bool dequeue(PriorityQueue *q, Node **n) {
    uint32_t size = pq_size(q);
    if (pq_empty(q)) {
        return false;
    }
    uint32_t index = q->indices[0];
    ns_push(q->available_indices, index);
    *n = q->nodes[index];
    q->nodes[index] = NULL; // very important, otherwise deleting stack could free nodes in use.
    for (uint32_t i = 0; i < size - 1; i++) {
        q->indices[i] = q->indices[i + 1];
    }
    q->indices[size - 1] = EMPTY;
    return true;
}

// Helper method that prints the PriorityQueue.
// NOTE: This will treat every node in the node array as the root of a tree, and print that tree.
// If multiple nodes in PriorityQueue belong to the same tree, this print function will print some
// nodes multiply times.
void pq_print(PriorityQueue *q) {
    for (uint32_t i = 0; i < pq_size(q); i++) {
        node_print(q->nodes[q->indices[i]]);
    }
    return;
}
