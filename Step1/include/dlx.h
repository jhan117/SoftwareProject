#pragma once

#include "dxl_types.h"
#include "solution_types.h"

#define MAX_COLS 9 * 9 * 4

void initDLX(DLX* dlx);
void addRow(DLX* dlx, int row_id, int* col_indices, int col_num);

void boardToDLX(DLX* dlx, int board[SIZE][SIZE]);

Column* chooseColumnWithMinSize(DLX* dlx);
void decodeSolution(int solution[], int size, int board[SIZE][SIZE]);
void cover(Column* c);
void uncover(Column* c);
void solveSudokuDLX(DLX* dlx, int solution[], int k, SudokuSolutions* sols);