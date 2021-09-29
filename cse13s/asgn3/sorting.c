#include "bubble.c"
#include "quick.c"
#include "set.c"
#include "shell.c"

#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
// These variables are linked with the extern keyword in other files in order to access this
// information.
long moves = 0;
long compares = 0;
long max_stack = 0;
long max_queue = 0;
// Helper method that prints an array to the screen, not used in final result.
void print_array(uint32_t *A, uint32_t n) {
    if (n == 0) {
        printf("[]\n");
        return;
    }
    printf("[%d", A[0]);
    for (uint32_t i = 1; i < n; i++) {
        printf(", %d", A[i]);
    }
    printf("]\n");
    return;
}

// Test Method that checks if an array is sorted, not used in final result.
bool is_sorted(uint32_t *A, uint32_t n) {
    if (n < 2) {
        return true;
    }
    for (uint32_t i = 1; i < n; i++) {
        if (A[i - 1] > A[i]) {
            return false;
        }
    }
    return true;
}
// Modified code given in assigment pdf that dynamically creates an array.
uint32_t *array_create(uint32_t capacity) {
    return (uint32_t *) calloc(capacity, sizeof(uint32_t));
}
// Modified code given in assignment pdf that frees a dynamically created array.
void array_delete(uint32_t **arr) {
    if (*arr) {
        free(*arr);
        *arr = NULL;
    }
    return;
}
// This code is called if the user wants to show bubble sort.
void bubble_behavior(unsigned int seed, uint32_t size, uint32_t elements) {
    uint32_t *arr = array_create(size);
    srand(seed);
    for (uint32_t i = 0; i < size; i++) {
        arr[i] = rand();
    }
    bubble_sort(arr, size);
    printf("Bubble Sort\n%" PRIu32 " elements, %ld moves, %ld compares", size, moves, compares);
    if (elements == 0) {
        printf("\n");
        return;
    }
    for (uint32_t i = 0; i < elements; i++) {
        if (i % 5 == 0) {
            printf("\n");
        }
        printf("%13" PRIu32 "\t", arr[i]);
    }
    printf("\n");
    array_delete(&arr);
    return;
}
// This code is called if the user wants to show shell sort.
void shell_behavior(unsigned int seed, uint32_t size, uint32_t elements) {
    uint32_t *arr = array_create(size);
    srand(seed);
    for (uint32_t i = 0; i < size; i++) {
        arr[i] = rand();
    }
    shell_sort(arr, size);
    printf("Shell Sort\n%" PRIu32 " elements, %ld moves, %ld compares", size, moves, compares);
    if (elements == 0) {
        printf("\n");
        return;
    }
    for (uint32_t i = 0; i < elements; i++) {
        if (i % 5 == 0) {
            printf("\n");
        }
        printf("%13" PRIu32 "\t", arr[i]);
    }
    printf("\n");
    array_delete(&arr);
    return;
}
// This code is called if the user wants to show quick sort with queue.
void quick_queue_behavior(unsigned int seed, uint32_t size, uint32_t elements) {
    uint32_t *arr = array_create(size);
    srand(seed);
    for (uint32_t i = 0; i < size; i++) {
        arr[i] = rand();
    }
    quick_sort_queue(arr, size);
    printf(
        "Quick Sort (Queue)\n%" PRIu32 " elements, %ld moves, %ld compares", size, moves, compares);
    printf("\nMax queue size: %ld", max_queue);
    if (elements == 0) {
        printf("\n");
        return;
    }
    for (uint32_t i = 0; i < elements; i++) {
        if (i % 5 == 0) {
            printf("\n");
        }
        printf("%13" PRIu32 "\t", arr[i]);
    }
    printf("\n");
    array_delete(&arr);
    return;
}
// This code is called if the user wants to show quick sort with stack.
void quick_stack_behavior(unsigned int seed, uint32_t size, uint32_t elements) {
    uint32_t *arr = array_create(size);
    srand(seed);
    for (uint32_t i = 0; i < size; i++) {
        arr[i] = rand();
    }
    quick_sort_stack(arr, size);
    printf(
        "Quick Sort (Stack)\n%" PRIu32 " elements, %ld moves, %ld compares", size, moves, compares);
    printf("\nMax stack size: %ld", max_stack);
    if (elements == 0) {
        printf("\n");
        return;
    }
    for (uint32_t i = 0; i < elements; i++) {
        if (i % 5 == 0) {
            printf("\n");
        }
        printf("%13" PRIu32 "\t", arr[i]);
    }
    printf("\n");
    array_delete(&arr);
    return;
}

// Main method that is run. Parses command line input and prints sorts to the screen with other
// data about the sorts, e.g. moves, comparisons, max queue size, and max stack size.
int main(int argc, char **argv) {
    enum sorts { b, s, q, Q } sort; // potential sorts that can be done, done to mask integer aspect
    // of set.
    Set set = set_empty();
    unsigned int seed = 13371453; // default seed
    uint32_t size = 100; //default size
    uint32_t elements = 100; // default elements
    int opt = 0;
    while ((opt = getopt(argc, argv, "absqQr:n:p:")) != -1) {
        switch (opt) {
        case 'b':
            sort = b;
            set = set_insert(set, sort);
            break;
        case 's':
            sort = s;
            set = set_insert(set, sort);
            break;
        case 'q':
            sort = q;
            set = set_insert(set, sort);
            break;
        case 'Q':
            sort = Q;
            set = set_insert(set, sort);
            break;
        case 'a':
            sort = b;
            set = set_insert(set, sort);
            sort = s;
            set = set_insert(set, sort);
            sort = q;
            set = set_insert(set, sort);
            sort = Q;
            set = set_insert(set, sort);
            break;
        case 'r': seed = atol(optarg); break; //atol converts string to long, comes from stdlib.
        case 'n': size = atol(optarg); break;
        case 'p': elements = atol(optarg); break;
        default: break;
        }
    }
    if (elements > size) {
        elements = size; //reduces elements to be printed done if it is higher than num of elements.
    }
    sort = b;
    if (set_member(set, sort)) {
        bubble_behavior(seed, size, elements);
    }
    sort = s;
    if (set_member(set, sort)) {
        shell_behavior(seed, size, elements);
    }
    sort = q;
    if (set_member(set, sort)) {
        quick_stack_behavior(seed, size, elements);
    }
    sort = Q;
    if (set_member(set, sort)) {
        quick_queue_behavior(seed, size, elements);
    }
    return 0;
}
