#pragma once

#include "solution_types.h"

int solveSudokuDFS(int board[SIZE][SIZE], SudokuSolutions *sols,
                   int *curr_calls);