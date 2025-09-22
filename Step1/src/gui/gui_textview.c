#include "gui/gui_textview.h"

#include "utils/board_utils.h"
#include "utils/parsing.h"
#include <gtk/gtk.h>

Status board_from_textview(GtkTextView *text_view, int board[SIZE][SIZE]) {
  // textview에서 내용 읽기
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);
  GtkTextIter start, end;
  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);
  gchar *text = gtk_text_buffer_get_text(buffer, &start, &end, TRUE);

  // 빈 내용
  if (!text)
    return FORMAT_ERROR;

  Status s = parse_board_from_text(text, board);
  g_free(text); // 메모리 해제

  return s;
}

void textview_from_board(GtkTextView *text_view, int board[SIZE][SIZE]) {
  // textview 초기화
  GtkTextBuffer *buffer = gtk_text_view_get_buffer(text_view);
  gtk_text_buffer_set_text(buffer, "", -1);

  // 보드 직렬화 (공백 포함)
  char text[SIZE * (SIZE * 2) + 1];
  serialize_board_for_ui(board, text);

  gtk_text_buffer_set_text(buffer, text, -1);
}