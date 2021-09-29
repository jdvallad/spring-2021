#include "ll.h"

#include "node.c"

#include <stdio.h>
#include <string.h>

struct LinkedList {
    uint32_t length;
    Node *head;
    Node *tail;
    bool mtf;
};

// Extern variables that are used to print statistics
extern uint64_t seeks;
extern uint64_t links;

// Constructor Function that creates a LinkedList. Dynamically allocated memory
// for the LinkedList and the sentinel head and tail nodes. Pointers in the head
// and tail nodes are modified so that they point to each other.
LinkedList *ll_create(bool mtf) {
    LinkedList *ll = (LinkedList *) malloc(sizeof(LinkedList));
    if (ll) {
        ll->length = 0;
        ll->head = node_create(NULL, NULL);
        ll->tail = node_create(NULL, NULL);
        ll->tail->prev = ll->head;
        ll->head->next = ll->tail;
        ll->mtf = mtf;
    }
    return ll;
}

// Destructor Function for LinkedList. Every node between head and tail are
// freed, then the memory for ll is freed. Finally, the pointer to ll is set to
// NULL.
//
// NOTE: If some node in ll has a next field of NULL and is not the tail node,
// then this will result in the dereferencing of a NULL pointer. Use with
// caution.
void ll_delete(LinkedList **ll) {
    if (*ll) {
        Node *n = (*ll)->head->next;
        while (n != (*ll)->tail) {
            node_delete(&(n->prev));
            n = n->next;
        }
        node_delete(&(n->prev));
        node_delete(&n);
        free(*ll);
        *ll = NULL;
    }
    return;
}

// Returns the length of LinkedList ll.
uint32_t ll_length(LinkedList *ll) {
    uint32_t count = 0;
    Node *n = ll->head->next;
    while (n != ll->tail) {
        count++;
        n = n->next;
        if (n == NULL) {
            break;
        }
    }
    return count;
}

// Moves Node n to the front of LinkedList ll, directly after the head Node. If
// already_in_list is true, then the Nodes pointed to by n's original prev and
// next fields are modified so that they point to each other, as if the hole
// made by removing n is being plugged.
void ll_put_node_in_front(LinkedList *ll, Node *n, bool already_in_list) {
    if (already_in_list) {
        n->prev->next = n->next;
        n->next->prev = n->prev;
    }
    n->next = ll->head->next;
    n->prev = ll->head;
    ll->head->next->prev = n;
    ll->head->next = n;
    return;
}

// Checks for the existence of a Node within ll that has the oldspeak field
// equal to the parameter oldspeak. If it does, then the Node is moved to the
// front of the LinkedList, if the move-to-front option has been set, and
// then the Node is returned. Othewise, NULL is returned.
Node *ll_lookup(LinkedList *ll, char *oldspeak) {
    seeks++;
    Node *n = ll->head->next;
    while (n != ll->tail) {
        if (n == NULL) {
            return NULL;
        }
        if (!strcmp(oldspeak, n->oldspeak)) {
            if (ll->mtf) {
                ll_put_node_in_front(ll, n, true);
            }
            return n;
        }
        n = n->next;
        links++;
    }
    return NULL;
}

// Attempts to insert a node with the fields oldspeak and newspeak into the
// LinkedList ll. If a node with these fields already exists, then that node is
// moved to front of the list if the move-to-front option has been enabled. If
// the node doesn't exist, a new node is created and inserted into the front of
// the list.
void ll_insert(LinkedList *ll, char *oldspeak, char *newspeak) {
    if (ll_lookup(ll, oldspeak)) {
        return;
    }
    ll_put_node_in_front(ll, node_create(oldspeak, newspeak), false);
    return;
}

// Helper Function that prints all Nodes in ll to stdout.
void ll_print(LinkedList *ll) {
    Node *n = ll->head->next;
    while (n != ll->tail) {
        if (n == NULL) {
            return;
        }
        node_print(n);
        n = n->next;
    }
    return;
}
