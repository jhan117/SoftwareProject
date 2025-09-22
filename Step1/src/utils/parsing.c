#include "utils/parsing.h"

static Status parse_row(gchar *line, int row, int board[SIZE][SIZE]) {
  if (!line)
    return FORMAT_ERROR;

  gchar *trimmed = g_strstrip(line);
  gchar **tokens = g_strsplit(trimmed, " ", 0);

  int col = 0;
  for (gchar **token = tokens; *token != NULL; token++, col++) {
    // 열 초과
    if (col >= SIZE) {
      g_strfreev(tokens);
      return FORMAT_ERROR;
    }

    gchar *trimmed_token = g_strstrip(*token);
    // 한자리만 가능
    if (strlen(trimmed_token) != 1) {
      g_strfreev(tokens);
      return FORMAT_ERROR;
    }

    if (g_ascii_isdigit(trimmed_token[0])) {
      // 숫자로 변환
      int val = trimmed_token[0] - '0';
      board[row][col] = val;
    } else {
      g_strfreev(tokens);
      return FORMAT_ERROR;
    }
  }

  g_strfreev(tokens);
  // 열 부족
  if (col != SIZE)
    return FORMAT_ERROR;
  return OK;
}

Status parse_board_from_text(gchar *text, int board[SIZE][SIZE]) {
  // 양끝 공백 제거 후 구분자 기준으로 나누기
  gchar *trimmed = g_strstrip(text);
  gchar **lines = g_strsplit(trimmed, "\n", 0);

  int row = 0;
  for (gchar **line = lines; *line != NULL; line++, row++) {
    // 행 초과
    if (row >= SIZE) {
      g_strfreev(lines);
      return FORMAT_ERROR;
    }

    // 열 parsing
    Status s = parse_row(*line, row, board);
    if (s != OK) {
      g_strfreev(lines);
      return FORMAT_ERROR;
    }
  }

  g_strfreev(lines);
  // 행 부족
  if (row != SIZE)
    return FORMAT_ERROR;

  return OK;
}