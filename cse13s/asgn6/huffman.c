#include "huffman.h"

#include "pq.c"
#include "stack.c"

#include <stdio.h>

// Constructs a huffman tree given a histogram of character distribution.
// Uses a priority queue to create a node for each character that has a positive frequency.
// Nodes are joined together based on their frequency until one node remains, the root of the
// huffman tree. This node is then returned.
Node *build_tree(uint64_t hist[static ALPHABET]) {
    PriorityQueue *pq = pq_create(ALPHABET);
    for (uint32_t i = 0; i < ALPHABET; i++) {
        if (hist[i] > 0) {
            enqueue(pq, node_create(i, hist[i]));
        }
    }
    Node *left;
    Node *right;
    while (pq_size(pq) >= 2) {
        dequeue(pq, &left);
        dequeue(pq, &right);
        enqueue(pq, node_join(left, right));
    }
    dequeue(pq, &left);
    pq_delete(&pq);
    return left;
}

// Recursive function that is called in build_codes in order to traverse the tree.
// traverse entirety of tree given by root.
// When leaves are reached, that leaf's code is added to alphabet, with the index being the leaf's
// symbol.
void recurse(Code c, Code table[static ALPHABET], Node *root) {
    if (root == NULL) {
        return;
    }
    uint8_t dummy = 0;
    code_push_bit(&c, 0);
    recurse(c, table, root->left);
    code_pop_bit(&c, &dummy);
    code_push_bit(&c, 1);
    recurse(c, table, root->right);
    code_pop_bit(&c, &dummy);
    if (node_is_leaf(root)) {
        table[root->symbol] = c;
    }
    return;
}
// traverses the huffman tree given by its root node, root. Populates code table with each leaf's
// corresponding code.
void build_codes(Node *root, Code table[static ALPHABET]) {
    for (int i = 0; i < ALPHABET; i++) {
        table[i] = code_init();
    }
    Code c = code_init();
    recurse(c, table, root);
    return;
}

// Takes in a tree dump given by an array of nbytes number of bytes. Returns the corresponding
// Huffman tree.
Node *rebuild_tree(uint16_t nbytes, uint8_t tree[static nbytes]) {
    Stack *s = stack_create(nbytes);
    Node *right = NULL;
    Node *left = NULL;
    for (uint16_t i = 0; i < nbytes; i++) {
        if (tree[i] == 'L') {
            i++;
            stack_push(s, node_create(tree[i], 1));
            continue;
        }
        // The requirement below for stack_size to be greater than 1 is not actually necessary as
        // this is guaranteed to be the case due to the nature of how the tree was constructed.
        // Since the tree was constructed with a post-order traversal, all nodes will be added first
        // and will be at the beginning of the array. I only put this here because
        // scan-build does not realize that this is how the tree is being constructed and thinks
        // that I am going to pop twice when there isn't enough elements to do so.
        if (stack_size(s) > 1 && tree[i] == 'I') {
            stack_pop(s, &right);
            stack_pop(s, &left);
            Node *res = node_join(left, right);
            stack_push(s, res);
            continue;
        }
    }
    stack_pop(s, &left);
    stack_delete(&s);
    return left;
}
// deletes tree given by root using a postorder traversal.
void delete_tree(Node **root) {
    if (*root == NULL) {
        return; //current node doesn't exist, nothing to do.
    }
    delete_tree(&((*root)->left)); //recurse down left branch.
    delete_tree(&((*root)->right)); //recurse down right branch.
    free(*root); //After traversing both branches, free current node.
    *root = NULL;
    return;
}
