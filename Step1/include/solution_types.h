#pragma once

#define SIZE 9
#define MAX_SOLUTIONS 1000

typedef struct {
    int count;
    int boards[MAX_SOLUTIONS][SIZE][SIZE];
} SudokuSolutions;