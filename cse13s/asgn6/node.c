#include "node.h"

#include <stdio.h>
#include <stdlib.h>

// Node Constructor
// initializes left and right child to NULL.
Node *node_create(uint8_t symbol, uint64_t frequency) {
    Node *n = (Node *) malloc(sizeof(Node));
    if (n) {
        n->left = NULL;
        n->right = NULL;
        n->symbol = symbol;
        n->frequency = frequency;
    }
    return n;
}

// Node destructor Method.
// DOES NOT RECURSIVELY FREE ALL CHILD NODES
// To do so, see delete_tree() in huffman.h
void node_delete(Node **n) {
    if (*n) {
        free(*n);
        *n = NULL;
    }
    return;
}

// Joins two nodes together into one node.
// Left node will be right child, Right child will be right child
// new symbol will be '$'
// frequency will be sum of child frequencies
// If any children are NULL, their frequencies are treated as 0
Node *node_join(Node *left, Node *right) {
    uint64_t left_freq = (left == NULL) ? 0 : left->frequency;
    uint64_t right_freq = (right == NULL) ? 0 : right->frequency;
    Node *n = node_create('$', left_freq + right_freq);
    n->left = left;
    n->right = right;
    return n;
}

// Helper method to check if node n is a leaf.
bool node_is_leaf(Node *n) {
    return n != NULL && n->left == NULL && n->right == NULL;
}

// Helper method to print a node. Treats node as a tree and recursively prints all of its children.
void node_print(Node *n) { //Traverses nodes as a binary tree with a postorder traversal
    if (n == NULL) {
        return; // Current Node doesn't exist
    }
    node_print(n->left); // Traverse left branch
    node_print(n->right); // Traverse right branch
    printf("(%c, %lu)\n", n->symbol,
        n->frequency); // After traversing both branches, print branch symbol and frequency
    return;
}
