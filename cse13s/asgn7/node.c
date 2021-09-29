#include "node.h"

#include <stdio.h>
#include <string.h>

// Helper Function that duplicates a string. The output string is dynamically
// allocated and will not be modified if the input string is modified. As it is
// dynamically allocated, it will also need to freed when it is no longer
// needed. Try to use sparingly.
char *string_duplicate(char *input) {
    int len = strlen(input) + 1; // This +1 is very important, needed for '\0'
    char *res = (char *) calloc(len, sizeof(char));
    strcpy(res, input);
    return res;
}

// Node Constructor Method. Dynamically allocated memory for a Node. Then
// dynamically allocated memory for the newspeak and oldspeak fields. Returns
// resulting Node.
Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = (Node *) malloc(sizeof(Node));
    n->oldspeak = NULL;
    n->newspeak = NULL;
    if (oldspeak != NULL) {
        n->oldspeak = string_duplicate(oldspeak);
    }
    if (newspeak != NULL) {
        n->newspeak = string_duplicate(newspeak);
    }
    n->next = NULL;
    n->prev = NULL;
    return n;
}

// Desructor Method for a Node. Frees memory for the oldspeak and newspeak
// fields, and then frees the memory for Node n. Finally, pointer to n is set to
// NULL.
void node_delete(Node **n) {
    if (*n) {
        if ((*n)->oldspeak) {
            free((*n)->oldspeak);
        }
        if ((*n)->newspeak) {
            free((*n)->newspeak);
        }
        free(*n);
        *n = NULL;
    }
    return;
}

// Helper Function that prints a Node to stdout. Slightly different print
// formatting is used depending on whether or not the newspeak field in n is
// NULL.
void node_print(Node *n) {
    if (n->oldspeak && n->newspeak) {
        printf("%s -> %s\n", n->oldspeak, n->newspeak);
    }
    if (n->oldspeak && !n->newspeak) {
        printf("%s\n", n->oldspeak);
    }
    return;
}
