// This file was used to generate data to create the graphs
// Pay to attention to this file.
// Unless you want to.
#include "quick.c"
#include "set.c"
#include "shell.c"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#define SIZE 1000

long compares = 0;
long moves = 0;
long max_stack = 0;
long max_queue = 0;

void write_to_file(char *file_path, uint32_t *input, uint32_t *output, uint32_t points) {
    FILE *fptr;
    fptr = fopen(file_path, "w");
    for (uint32_t i = 0; i < points; i++) {
        fprintf(fptr, "%" PRIu32 "\t%" PRIu32 "\n", input[i], output[i]);
    }
    fclose(fptr);
    return;
}
void append_to_file(char *file_path, uint32_t *input, uint32_t *output, uint32_t points) {
    FILE *fptr;
    fptr = fopen(file_path, "a");
    for (uint32_t i = 0; i < points; i++) {
        fprintf(fptr, "%" PRIu32 "\t%" PRIu32 "\n", input[i], output[i]);
    }
    fclose(fptr);
    return;
}
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
uint32_t *array_create(uint32_t capacity) {
    return (uint32_t *) calloc(capacity, sizeof(uint32_t));
}
void array_delete(uint32_t **arr) {
    if (*arr) {
        free(*arr);
        *arr = NULL;
    }
    return;
}

void randomize(uint32_t *arr, uint32_t size) {
    while (size > 0) {
        size--;
        arr[size] = random();
    }
    return;
}
uint32_t *random_array(int size) {
    uint32_t *arr = array_create(size);
    randomize(arr, size);
    return arr;
}
void reverse_array(uint32_t *arr, uint32_t size) {
    for (uint32_t i = 0; i < size / 2; i++) {
        uint32_t temp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = temp;
    }
    return;
}
uint32_t *reverse_sorted_array(uint32_t size) {
    uint32_t *arr = array_create(size);
    randomize(arr, size);
    quick_sort_stack(arr, size);
    reverse_array(arr, size);
    return arr;
}
int main(void) {
    uint32_t comparisons[SIZE];
    uint32_t move_count[SIZE];
    //uint32_t stack_max[SIZE];
    //uint32_t queue_max[SIZE];
    uint32_t ind[SIZE];
    for (int i = 1; i <= SIZE; i++) {
        ind[i - 1] = i;
        uint32_t *arr = reverse_sorted_array(i);
        bubble_sort(arr, i);
        comparisons[i - 1] = compares;
        move_count[i - 1] = moves;
        //stack_max[i - 1] = max_stack;
        //queue_max[i - 1] = max_queue;
        array_delete(&arr);
    }
    write_to_file("./writeup/r_bubble_sort_comparisons.txt", ind, comparisons, SIZE);
    write_to_file("./writeup/r_bubble_sort_moves.txt", ind, move_count, SIZE);
    //write_to_file("./writeup/r_quick_sort_queue_max.txt",ind,queue_max,SIZE);
    // write_to_file("./writeup/r_quick_sort_stack_max.txt",ind,stack_max,SIZE);
    return 0;
}
