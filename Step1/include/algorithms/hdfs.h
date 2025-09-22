#pragma once

#include "hdfs_types.h"
#include "solution_types.h"

void initBoard(SudokuBoard *board, int input[SIZE][SIZE]);
int solveSudokuHDFS(SudokuBoard *board, SudokuSolutions *sols, int *curr_calls);