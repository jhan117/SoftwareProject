#include "app_context.h"

void reset_context(AppContext *ctx) {
  // 현재 보드 초기화
  memset(ctx->cur_board, 0, sizeof(ctx->cur_board));

  // solutions 초기화
  if (ctx->sols) {
    g_free(ctx->sols);
    ctx->sols = NULL;
  }
  ctx->current_idx = 0;

  // page label 초기화
  gtk_label_set_text(GTK_LABEL(ctx->label_page), "0 / 0");

  // result table 선택 창 초기화
  if (ctx->result_table) {
    GtkTreeSelection *selection =
        gtk_tree_view_get_selection(ctx->result_table);
    gtk_tree_selection_unselect_all(selection);
  }
}