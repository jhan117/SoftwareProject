#include "algorithms/dfs.h"

static int findEmptyCell(int board[SIZE][SIZE], int *row, int *col) {
  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE; j++)
      if (board[i][j] == 0) {
        *row = i;
        *col = j;
        return 1;
      }
  return 0;
}

static int isValid(int board[SIZE][SIZE], int row, int col, int num) {
  for (int i = 0; i < SIZE; i++) {
    if (board[row][i] == num)
      return 0;
    if (board[i][col] == num)
      return 0;
  }
  int startRow = row - row % 3;
  int startCol = col - col % 3;
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      if (board[startRow + i][startCol + j] == num)
        return 0;
  return 1;
}

int solveSudokuDFS(int board[SIZE][SIZE], SudokuSolutions *sols,
                   int *curr_calls) {
  (*curr_calls)++;

  int row, col;
  if (!findEmptyCell(board, &row, &col)) {
    if (sols->count < MAX_SOLUTIONS) {
      for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
          sols->boards[sols->count][i][j] = board[i][j];

      sols->dfs_calls[sols->count] = *curr_calls;

      sols->count++;
    }
    return 1;
  }

  for (int num = 1; num <= 9; num++) {
    if (isValid(board, row, col, num)) {
      board[row][col] = num;
      solveSudokuDFS(board, sols, curr_calls);
      board[row][col] = 0;
    }
  }

  return 0;
}