#include "utils/board_utils.h"
#include <gtk/gtk.h>

const char *level_to_string(int level) {
  switch (level) {
  case 0:
    return "Easy";
  case 1:
    return "Normal";
  case 2:
    return "Hard";
  default:
    return "N/A";
  }
}

void serialize_board_for_ui(int board[SIZE][SIZE], char out[SIZE * SIZE + 1]) {
  int k = 0;
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      out[k++] = board[i][j] + '0';
      out[k++] = ' '; // 공백으로 구분!
    }
    out[k - 1] = '\n'; // 마지막 공백을 줄바꿈으로 대체
  }
  out[k] = '\0';
}

void serialize_board(int board[SIZE][SIZE], char out[SIZE * SIZE + 1]) {
  int k = 0;
  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE; j++)
      // 문자 변환
      out[k++] = board[i][j] + '0';
  // 널문자 추가
  out[k] = '\0';
}

GString *serialize_all_solutions(SudokuSolutions *sols) {
  GString *result = g_string_new("");
  for (int i = 0; i < sols->count; i++) {
    char temp[SIZE * SIZE + 1];
    serialize_board(sols->boards[i], temp);
    g_string_append(result, temp);
    if (i != sols->count - 1)
      g_string_append_c(result, '|'); // 구분자 넣기
  }
  return result;
}

void board_from_string(const char *str, int board[SIZE][SIZE]) {
  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE; j++)
      board[i][j] = str[i * SIZE + j] - '0';
}