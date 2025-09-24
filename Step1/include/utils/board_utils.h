#pragma once

#include "common.h"
#include "solution_types.h"
#include <glib.h>

const char *level_to_string(int level);
int string_to_level(const char *level);
void serialize_board_for_ui(int board[SIZE][SIZE], char out[SIZE * SIZE + 1]);
void serialize_board(int board[SIZE][SIZE], char out[SIZE * SIZE + 1]);
GString *serialize_all_solutions(SudokuSolutions *sols);
void board_from_string(const char *str, int board[SIZE][SIZE]);
