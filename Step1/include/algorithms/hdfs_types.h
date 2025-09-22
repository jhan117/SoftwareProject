#pragma once

#include "common.h"

typedef struct {
  int value;
  int candidates[SIZE];
} Cell;

typedef struct {
  Cell cells[SIZE][SIZE];
  int empty_count;
} SudokuBoard;

typedef struct {
  int row, col, num, prev;
} CandidateChange;