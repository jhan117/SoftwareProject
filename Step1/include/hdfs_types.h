#pragma once

#define SIZE 9

typedef struct {
    int value;   
    int candidates[SIZE];
} Cell;

typedef struct {
    Cell cells[SIZE][SIZE];
    int empty_count;
} SudokuBoard;

typedef struct {
    int row, col, num, prev; // prev는 이전 후보 값
} CandidateChange;