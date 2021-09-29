#include "bubble.h"

#include <stdbool.h>

//Pseudocode given in Assignment PDF
void bubble_sort(uint32_t *A, uint32_t n) {
    extern long moves;
    extern long compares;
    moves = 0;
    compares = 0;
    bool swapped = true;
    uint32_t temp = 0;
    while (swapped) {
        swapped = false;
        for (uint32_t i = 1; i < n; i++) {
            if (A[i] < A[i - 1]) {
                temp = A[i];
                A[i] = A[i - 1];
                A[i - 1] = temp;
                swapped = true;
                moves += 3;
            }
            compares++;
        }
        n--;
    }
    return;
}
