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
  gchar *init_str = NULL, *sol_str = NULL, *diff_str = NULL, *algo_str = NULL;
  gint num_sols = 0;
  gtk_tree_model_get(model, &iter, 1, &algo_str, 2, &diff_str, 3, &num_sols, 6,
                     &init_str, 7, &sol_str, -1);

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

  // 콤보박스 처리
  int algo_num;
  if (strcmp(algo_str, "DFS") == 0) {
    algo_num = 0;
  } else if (strcmp(algo_str, "HDFS") == 0) {
    algo_num = 1;
  } else if (strcmp(algo_str, "DLX") == 0) {
    algo_num = 2;
  }
  gtk_combo_box_set_active(GTK_COMBO_BOX(ctx->level_combo),
                           ctx->difficulty_level);
  gtk_combo_box_set_active(GTK_COMBO_BOX(ctx->algo_combo), algo_num);

  // 메모리 해제
  g_free(init_str);
  g_free(sol_str);
}

static void on_delete_row(GtkMenuItem *menuitem, gpointer user_data) {
  AppContext *ctx = (AppContext *)user_data;

  GtkTreeSelection *selection = gtk_tree_view_get_selection(ctx->result_table);
  GtkTreeModel *model = gtk_tree_view_get_model(ctx->result_table);
  GtkTreeIter iter;

  if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
    // 현재 path 저장
    GtkTreePath *path = gtk_tree_model_get_path(model, &iter);
    int index = gtk_tree_path_get_indices(path)[0];

    gtk_list_store_remove(GTK_LIST_STORE(model), &iter);

    // 삭제 후 선택
    GtkTreeIter new_iter;
    GtkTreePath *new_path = NULL;

    // 다음 행 있니?
    if (gtk_tree_path_new_from_indices(index, -1)) {
      new_path = gtk_tree_path_new_from_indices(index, -1);
      if (!gtk_tree_model_get_iter(model, &new_iter, new_path)) {
        gtk_tree_path_free(new_path);
        new_path = NULL;
      }
    }

    // 이전 행 있니?
    if (!new_path && index > 0) {
      new_path = gtk_tree_path_new_from_indices(index - 1, -1);
      if (!gtk_tree_model_get_iter(model, &new_iter, new_path)) {
        gtk_tree_path_free(new_path);
        new_path = NULL;
      }
    }

    if (new_path) {
      gtk_tree_selection_select_iter(selection, &new_iter);
      gtk_tree_path_free(new_path);
    } else {
      reset_context(ctx);
      display_solution(ctx, NULL, 0);
      GtkTextBuffer *buffer =
          gtk_text_view_get_buffer(GTK_TEXT_VIEW(ctx->input_textview));
      gtk_text_buffer_set_text(buffer, "", -1);
    }

    gtk_tree_path_free(path);
  }
}

gboolean on_treeview_right_click(GtkWidget *tree_view, GdkEventButton *event,
                                 gpointer user_data) {
  AppContext *ctx = (AppContext *)user_data;

  if (event->type == GDK_BUTTON_PRESS &&
      event->button == GDK_BUTTON_SECONDARY) { // GDK_BUTTON_SECONDARY: 우클릭
    GtkTreePath *path = NULL;
    if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(tree_view), (gint)event->x,
                                      (gint)event->y, &path, NULL, NULL,
                                      NULL)) {
      GtkTreeSelection *selection =
          gtk_tree_view_get_selection(GTK_TREE_VIEW(tree_view));
      gtk_tree_selection_unselect_all(selection); // 기존 선택된거 해제
      gtk_tree_selection_select_path(selection,
                                     path); // 현재 마우스 위치를 선택
      gtk_tree_path_free(path);
    }

    GtkWidget *menu = gtk_menu_new();
    GtkWidget *delete_item = gtk_menu_item_new_with_label("Delete Row");
    g_signal_connect(delete_item, "activate", G_CALLBACK(on_delete_row), ctx);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), delete_item);
    gtk_widget_show_all(menu);
    gtk_menu_popup_at_pointer(GTK_MENU(menu), (GdkEvent *)event);

    return TRUE;
  }
  return FALSE;
}
