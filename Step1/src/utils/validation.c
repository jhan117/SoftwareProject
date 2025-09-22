#include "utils/validation.h"

Status validate_row(int board[SIZE][SIZE], int row) {
  int seen[SIZE + 1] = {0};
  for (int col = 0; col < SIZE; col++) {
    int val = board[row][col];
    if (val && seen[val])
      return RULE_ERROR;
    seen[val] = val ? 1 : 0;
  }
  return OK;
}
Status validate_col(int board[SIZE][SIZE], int col) {
  int seen[SIZE + 1] = {0};
  for (int row = 0; row < SIZE; row++) {
    int val = board[row][col];
    if (val && seen[val])
      return RULE_ERROR;
    seen[val] = val ? 1 : 0;
  }
  return OK;
}
Status validate_block(int board[SIZE][SIZE], int start_row, int start_col) {
  int seen[SIZE + 1] = {0};
  for (int r = 0; r < 3; r++)
    for (int c = 0; c < 3; c++) {
      int val = board[start_row + r][start_col + c];
      if (val && seen[val])
        return RULE_ERROR;
      seen[val] = val ? 1 : 0;
    }
  return OK;
}
Status validate_board(int board[SIZE][SIZE]) {
  for (int i = 0; i < SIZE; i++) {
    if (validate_row(board, i) != OK)
      return RULE_ERROR;
    if (validate_col(board, i) != OK)
      return RULE_ERROR;
  }
  for (int br = 0; br < SIZE; br += 3)
    for (int bc = 0; bc < SIZE; bc += 3)
      if (validate_block(board, br, bc) != OK)
        return RULE_ERROR;
  return OK;
}