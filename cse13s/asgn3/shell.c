#include "shell.h"

#include "gaps.h"
extern long moves;
extern long compares;

//Pseudocode provided in Assignment PDF.
void shell_sort(uint32_t *A, uint32_t n) {
    uint32_t j = 0;
    uint32_t temp = 0;
    moves = 0;
    compares = 0;
    for (uint32_t k = 0, gap = gaps[0]; k < GAPS; gap = gaps[++k]) {
        for (uint32_t i = gap; i < n; i++) {
            compares++;
            j = i;
            temp = A[i];
            moves++;
            while (j >= gap && temp < A[j - gap]) {
                compares++;
                A[j] = A[j - gap];
                moves++;
                j -= gap;
            }
            A[j] = temp;
            moves++;
        }
    }
    return;
}
