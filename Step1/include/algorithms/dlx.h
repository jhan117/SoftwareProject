#pragma once

#include "dlx_types.h"
#include "solution_types.h"

void initDLX(DLX *dlx);
void boardToDLX(DLX *dlx, int board[SIZE][SIZE]);

void solveSudokuDLX(DLX *dlx, int solution[], int k, SudokuSolutions *sols,
                    int *node_visits);