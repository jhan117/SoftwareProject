#pragma once

#include "solution_types.h"

int findEmptyCell(int board[SIZE][SIZE], int* row, int* col);
int isValid(int board[SIZE][SIZE], int row, int col, int num);
int solveSudokuDFS(int board[SIZE][SIZE], SudokuSolutions* sols);