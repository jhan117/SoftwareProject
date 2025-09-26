#include "gui/gui_callbacks_test.h"

#include "generator.h"
#include "gui/gui_callbacks_run.h"
#include "gui/gui_textview.h"

static void set_buttons_sensitive(AppContext *ctx, gboolean state) {
  gtk_widget_set_sensitive(ctx->btn_gen, state);
  gtk_widget_set_sensitive(ctx->btn_prev, state);
  gtk_widget_set_sensitive(ctx->btn_next, state);
  gtk_widget_set_sensitive(ctx->btn_run, state);
  gtk_widget_set_sensitive(ctx->btn_test, state);
  gtk_widget_set_sensitive(ctx->btn_save, state);

  gtk_widget_set_sensitive(ctx->level_combo, state);
  gtk_widget_set_sensitive(ctx->algo_combo, state);
}

static gboolean step_task(gpointer data) {
  TaskData *t = data;

  int problem_idx = 5 - t->repeat;

  gtk_combo_box_set_active(GTK_COMBO_BOX(t->ctx->level_combo), t->level);
  gtk_combo_box_set_active(GTK_COMBO_BOX(t->ctx->algo_combo), t->algo);

  memcpy(t->ctx->cur_board, t->saved_boards[t->level][problem_idx],
         sizeof(t->ctx->cur_board));
  textview_from_board(GTK_TEXT_VIEW(t->ctx->input_textview), t->ctx->cur_board);
  t->ctx->difficulty_level = t->level;

  on_run_clicked(NULL, t->ctx);

  t->repeat--;
  if (t->repeat <= 0) {
    t->algo++;
    t->repeat = 5;
    if (t->algo >= 3) { // 다음 레벨
      t->level++;
      t->algo = 0;
    }
  }

  if (t->level >= 3) {
    set_buttons_sensitive(t->ctx, TRUE);
    g_free(t);
    return FALSE; // 반복 종료
  }

  g_idle_add(step_task, t);
  return FALSE;
}

void on_test_clicked(GtkButton *button, gpointer user_data) {
  TaskData *t = g_new(TaskData, 1);
  t->ctx = user_data;
  t->level = 0;
  t->algo = 0;
  t->repeat = 5;

  // 미리 생성
  for (int lvl = 0; lvl < 3; lvl++) {
    for (int i = 0; i < 5; i++) {
      generateRandomSudoku(t->saved_boards[lvl][i], lvl);
    }
  }

  set_buttons_sensitive(t->ctx, FALSE); // 혹시 모를 오류를 위해... disable
  g_idle_add(step_task, t);             // 사이클마다 실행
}