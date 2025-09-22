#pragma once

#include "app_context.h"

Status validate_row(int board[SIZE][SIZE], int row);
Status validate_col(int board[SIZE][SIZE], int col);
Status validate_block(int board[SIZE][SIZE], int start_row, int start_col);
Status validate_board(int board[SIZE][SIZE]);