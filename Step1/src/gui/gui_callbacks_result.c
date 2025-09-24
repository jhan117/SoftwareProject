#include "gui/gui_callbacks_result.h"

#include "app_context.h"
#include "gui/gui_callbacks_generate.h"
#include "gui/gui_grid.h"
#include "gui/gui_textview.h"
#include "utils/board_utils.h"

void on_result_selection_changed(GtkTreeSelection *selection,
                                 gpointer user_data) {
  AppContext *ctx = (AppContext *)user_data;

  // 선택된 행 데이터 읽기
  GtkTreeIter iter;
  GtkTreeModel *model;
  if (!gtk_tree_selection_get_selected(selection, &model, &iter))
    return;
  gchar *init_str = NULL, *sol_str = NULL, *diff_str = NULL;
  gint num_sols = 0;
  gtk_tree_model_get(model, &iter, 2, &diff_str, 3, &num_sols, 6, &init_str, 7,
                     &sol_str, -1);

  // 행 데이터 기반으로 보드판 만들기
  board_from_string(init_str, ctx->cur_board);
  ctx->sols = g_malloc0(sizeof(SudokuSolutions));
  ctx->sols->count = num_sols;
  ctx->difficulty_level = string_to_level(diff_str);
  // 솔루션 보드 처리
  gchar **sols_split = g_strsplit(sol_str, "|", -1);
  for (int i = 0; i < ctx->sols->count; i++) {
    board_from_string(sols_split[i], ctx->sols->boards[i]);
  }
  g_strfreev(sols_split);
  ctx->current_idx = 0;
  display_solution(ctx, ctx->sols, ctx->current_idx);

  // 원본 퍼즐 textview에 표시
  GtkTextBuffer *buffer =
      gtk_text_view_get_buffer(GTK_TEXT_VIEW(ctx->input_textview));
  g_signal_handlers_block_by_func(buffer, G_CALLBACK(on_textview_changed), ctx);
  textview_from_board(GTK_TEXT_VIEW(ctx->input_textview), ctx->cur_board);
  g_signal_handlers_unblock_by_func(buffer, G_CALLBACK(on_textview_changed),
                                    ctx);

  // 라벨 갱신
  gchar buf[16];
  g_snprintf(buf, sizeof(buf), "1 / %d", ctx->sols->count);
  gtk_label_set_text(GTK_LABEL(ctx->label_page), buf);

  // 메모리 해제
  g_free(init_str);
  g_free(sol_str);
}