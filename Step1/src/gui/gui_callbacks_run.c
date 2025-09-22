#include "gui/gui_callbacks_run.h"

#include "app_context.h"
#include "gui/gui_dialog.h"
#include "gui/gui_grid.h"
#include "gui/gui_textview.h"
#include "utils/board_utils.h"
#include "utils/solver.h"
#include "utils/validation.h"
#include <time.h>

static GtkTreeIter add_result_row(AppContext *ctx, const char *algorithm,
                                  const char *difficulty, double elapsed_ms) {
  GtkTreeIter iter;
  gtk_list_store_append(ctx->result_store, &iter);

  // Puzzle Id
  int puzzle_id = ctx->result_store_count + 1;
  // numSolutions
  int num_solutions = ctx->sols ? ctx->sols->count : 0;
  // avg calls
  int avg_calls = average_metric(algorithm, ctx->sols);
  // Initial board
  char init_str[SIZE * SIZE + 1];
  serialize_board(ctx->cur_board, init_str);
  // Solution boards
  GString *sols_str = serialize_all_solutions(ctx->sols);

  // 0. Puzzle ID 1. Algorithm 2. Difficulty 3. NumSolutions 4. avg call/nodes
  // 5. avg step filled 6. elapsed time 7. initial board 8. solution boards
  gtk_list_store_set(ctx->result_store, &iter, 0, puzzle_id, 1, algorithm, 2,
                     difficulty, 3, num_solutions, 4, avg_calls, 5, elapsed_ms,
                     6, g_strdup(init_str), 7, g_strdup(sols_str->str), -1);
  g_string_free(sols_str, TRUE);
  ctx->result_store_count++;
  return iter;
}
void on_run_clicked(GtkButton *button, gpointer user_data) {
  clock_t start, end;
  double elapsed_ms;
  // context 구조체 사용하여 전역 상태 관리
  AppContext *ctx = (AppContext *)user_data;
  reset_context(ctx);
  int board[SIZE][SIZE] = {0};

  // textview에서 parsing 및 validation
  Status parsing_status =
      board_from_textview(GTK_TEXT_VIEW(ctx->input_textview), board);
  if (parsing_status != OK) {
    reset_context(ctx);
    display_solution(ctx, NULL, 0);
    show_error_dialog(ctx, parsing_status);
    return;
  }
  Status valid_status = validate_board(board);
  if (valid_status != OK) {
    reset_context(ctx);
    display_solution(ctx, NULL, 0);
    show_error_dialog(ctx, valid_status);
    return;
  }

  // 보드 context에 저장하고 알고리즘 실행
  memcpy(ctx->cur_board, board, sizeof(board));
  int algo = gtk_combo_box_get_active(GTK_COMBO_BOX(ctx->algo_combo));
  start = clock();
  ctx->sols = run_algorithm(board, algo);
  end = clock();
  elapsed_ms = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;

  // 결과표에 저장
  const char *difficulty_str = level_to_string(ctx->difficulty_level);
  GtkTreeIter iter = add_result_row(
      ctx,
      gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(ctx->algo_combo)),
      difficulty_str, elapsed_ms);

  // 결과창에서 현재 결과 선택 + 스크롤
  GtkTreeSelection *selection = gtk_tree_view_get_selection(ctx->result_table);
  gtk_tree_selection_select_iter(selection, &iter);
  GtkTreePath *path =
      gtk_tree_model_get_path(GTK_TREE_MODEL(ctx->result_store), &iter);
  gtk_tree_view_scroll_to_cell(ctx->result_table, path, NULL, FALSE, 0.5, 0.0);
  gtk_tree_path_free(path);
}
static void update_page(AppContext *ctx, int new_idx) {
  if (!ctx->sols || new_idx < 0 || new_idx >= ctx->sols->count)
    return;

  ctx->current_idx = new_idx;
  display_solution(ctx, ctx->sols, ctx->current_idx);

  gchar buf[16];
  g_snprintf(buf, sizeof(buf), "%d / %d", ctx->current_idx + 1,
             ctx->sols->count);
  gtk_label_set_text(GTK_LABEL(ctx->label_page), buf);
}
void on_prev_clicked(GtkButton *button, gpointer user_data) {
  AppContext *ctx = (AppContext *)user_data;
  update_page(ctx, ctx->current_idx - 1);
}
void on_next_clicked(GtkButton *button, gpointer user_data) {
  AppContext *ctx = (AppContext *)user_data;
  update_page(ctx, ctx->current_idx + 1);
}
