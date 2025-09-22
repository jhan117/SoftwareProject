#include "gui/gui_callbacks_generate.h"

#include "app_context.h"
#include "generator.h"
#include "gui/gui_grid.h"
#include "gui/gui_textview.h"

void on_generate_clicked(GtkButton *button, gpointer user_data) {
  AppContext *ctx = (AppContext *)user_data;
  reset_context(ctx);
  int board[SIZE][SIZE];

  // 난이도를 가져오고 보드 랜덤 생성
  int level = gtk_combo_box_get_active(GTK_COMBO_BOX(ctx->level_combo));
  generateRandomSudoku(board, level);

  ctx->difficulty_level = level;

  // 생성된 보드를 context에 저장
  memcpy(ctx->cur_board, board, sizeof(board));

  // 사용자 입력으로 처리하는 거 혼동 방지 후 textview에 표시
  GtkTextBuffer *buffer =
      gtk_text_view_get_buffer(GTK_TEXT_VIEW(ctx->input_textview));
  g_signal_handlers_block_by_func(buffer, G_CALLBACK(on_textview_changed), ctx);
  textview_from_board(GTK_TEXT_VIEW(ctx->input_textview), board);
  g_signal_handlers_unblock_by_func(buffer, G_CALLBACK(on_textview_changed),
                                    ctx);

  // 바로 grid 위젯에 표시
  display_solution(ctx, NULL, 0);

  // 결과 창 선택 행 초기화
  GtkTreeSelection *selection = gtk_tree_view_get_selection(ctx->result_table);
  gtk_tree_selection_unselect_all(selection);
}

void on_textview_changed(GtkTextBuffer *buffer, gpointer user_data) {
  AppContext *ctx = (AppContext *)user_data;
  ctx->difficulty_level = -1;
}