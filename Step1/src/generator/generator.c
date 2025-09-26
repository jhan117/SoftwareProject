#include "generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void swapRandomRowsInBlock(int board[SIZE][SIZE]) {
  int block = rand() % 3;
  int r1 = rand() % 3;
  int r2 = rand() % 3;
  while (r1 == r2)
    r2 = rand() % 3;
  int row1 = block * 3 + r1;
  int row2 = block * 3 + r2;
  for (int i = 0; i < SIZE; i++) {
    int tmp = board[row1][i];
    board[row1][i] = board[row2][i];
    board[row2][i] = tmp;
  }
}

void swapRandomColsInBlock(int board[SIZE][SIZE]) {
  int block = rand() % 3;
  int c1 = rand() % 3;
  int c2 = rand() % 3;
  while (c1 == c2)
    c2 = rand() % 3;
  int col1 = block * 3 + c1;
  int col2 = block * 3 + c2;
  for (int i = 0; i < SIZE; i++) {
    int tmp = board[i][col1];
    board[i][col1] = board[i][col2];
    board[i][col2] = tmp;
  }
}

void swapRandomRowBlocks(int board[SIZE][SIZE]) {
  int b1 = rand() % 3;
  int b2 = rand() % 3;
  while (b1 == b2)
    b2 = rand() % 3;
  for (int i = 0; i < 3; i++) {
    int r1 = b1 * 3 + i;
    int r2 = b2 * 3 + i;
    for (int j = 0; j < SIZE; j++) {
      int tmp = board[r1][j];
      board[r1][j] = board[r2][j];
      board[r2][j] = tmp;
    }
  }
}

void swapRandomColBlocks(int board[SIZE][SIZE]) {
  int b1 = rand() % 3;
  int b2 = rand() % 3;
  while (b1 == b2)
    b2 = rand() % 3;
  for (int i = 0; i < 3; i++) {
    int c1 = b1 * 3 + i;
    int c2 = b2 * 3 + i;
    for (int j = 0; j < SIZE; j++) {
      int tmp = board[j][c1];
      board[j][c1] = board[j][c2];
      board[j][c2] = tmp;
    }
  }
}

void rotateBoard(int board[SIZE][SIZE]) {
  int temp[SIZE][SIZE];
  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE; j++)
      temp[j][SIZE - 1 - i] = board[i][j];
  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE; j++)
      board[i][j] = temp[i][j];
}

void mirrorBoard(int board[SIZE][SIZE]) {
  int axis = rand() % 2;
  if (axis == 0) {
    for (int i = 0; i < SIZE / 2; i++)
      for (int j = 0; j < SIZE; j++) {
        int tmp = board[i][j];
        board[i][j] = board[SIZE - 1 - i][j];
        board[SIZE - 1 - i][j] = tmp;
      }
  } else {
    for (int i = 0; i < SIZE; i++)
      for (int j = 0; j < SIZE / 2; j++) {
        int tmp = board[i][j];
        board[i][j] = board[i][SIZE - 1 - j];
        board[i][SIZE - 1 - j] = tmp;
      }
  }
}

void replaceNumbers(int board[SIZE][SIZE]) {
  int mapping[10];
  for (int i = 1; i <= 9; i++)
    mapping[i] = i;

  for (int i = 1; i <= 9; i++) {
    int j = 1 + rand() % 9;
    int tmp = mapping[i];
    mapping[i] = mapping[j];
    mapping[j] = tmp;
  }
  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE; j++)
      board[i][j] = mapping[board[i][j]];
}

void generateRandomSudoku(int board[SIZE][SIZE], int level) {
  int solved_board[SIZE][SIZE] = {
      {5, 3, 4, 6, 7, 8, 9, 1, 2}, {6, 7, 2, 1, 9, 5, 3, 4, 8},
      {1, 9, 8, 3, 4, 2, 5, 6, 7}, {8, 5, 9, 7, 6, 1, 4, 2, 3},
      {4, 2, 6, 8, 5, 3, 7, 9, 1}, {7, 1, 3, 9, 2, 4, 8, 5, 6},
      {9, 6, 1, 5, 3, 7, 2, 8, 4}, {2, 8, 7, 4, 1, 9, 6, 3, 5},
      {3, 4, 5, 2, 8, 6, 1, 7, 9}};

  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE; j++)
      board[i][j] = solved_board[i][j];

  int min_ops, max_ops, blanks;
  switch (level) {
  case 0:
    min_ops = 10;
    max_ops = 15;
    blanks = 30;
    break;
  case 1:
    min_ops = 15;
    max_ops = 25;
    blanks = 40;
    break;
  case 2:
    min_ops = 20;
    max_ops = 35;
    blanks = 50;
    break;
  default:
    min_ops = 15;
    max_ops = 25;
    blanks = 40;
    break;
  }

  int ops = min_ops + rand() % (max_ops - min_ops + 1);

  for (int i = 0; i < ops; i++) {
    int operation = rand() % 7;
    switch (operation) {
    case 0:
      swapRandomRowsInBlock(board);
      break;
    case 1:
      swapRandomColsInBlock(board);
      break;
    case 2:
      swapRandomRowBlocks(board);
      break;
    case 3:
      swapRandomColBlocks(board);
      break;
    case 4:
      rotateBoard(board);
      break;
    case 5:
      mirrorBoard(board);
      break;
    case 6:
      replaceNumbers(board);
      break;
    }
  }

  while (blanks > 0) {
    int r = rand() % SIZE;
    int c = rand() % SIZE;
    if (board[r][c] != 0) {
      board[r][c] = 0;
      blanks--;
    }
  }
}
