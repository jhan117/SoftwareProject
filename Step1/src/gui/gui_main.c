#include "gui/gui.h"

#include "gui/gui_callbacks_generate.h"
#include "gui/gui_callbacks_result.h"
#include "gui/gui_callbacks_run.h"
#include "gui/gui_callbacks_save.h"
#include "gui/gui_callbacks_test.h"
#include "gui/gui_grid.h"
#include "gui/gui_result.h"

#include <gtk/gtk.h>

static AppContext *app_context_new(GtkWindow *window) {
  AppContext *ctx = g_new0(AppContext, 1);
  ctx->window = window;
  return ctx;
}

static void init_first_column(AppContext *ctx, GtkBox *parent) {
  GtkWidget *label = gtk_label_new("Sudoku Input");

  // sudoku input window
  ctx->input_textview = gtk_text_view_new();
  GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(scrolled_window), ctx->input_textview);

  // level selection
  ctx->level_combo = gtk_combo_box_text_new();
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(ctx->level_combo), "Easy");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(ctx->level_combo),
                                 "Normal");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(ctx->level_combo), "Hard");
  gtk_combo_box_set_active(GTK_COMBO_BOX(ctx->level_combo), 1);

  // generate random sudoku button
  ctx->btn_gen = gtk_button_new_with_label("Generate Sudoku");

  // 배치
  gtk_box_pack_start(GTK_BOX(parent), label, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(parent), scrolled_window, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(parent), ctx->level_combo, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(parent), ctx->btn_gen, FALSE, FALSE, 0);

  // callback: on_generate_clicked, on_textview_changed
  g_signal_connect(ctx->btn_gen, "clicked", G_CALLBACK(on_generate_clicked),
                   ctx);
  GtkTextBuffer *buffer =
      gtk_text_view_get_buffer(GTK_TEXT_VIEW(ctx->input_textview));
  g_signal_connect(buffer, "changed", G_CALLBACK(on_textview_changed), ctx);
}

static void init_second_column(AppContext *ctx, GtkBox *parent) {
  // 스도쿠 출력 그리드
  create_grid(ctx);

  // 해답 페이지 이동 관련 위젯들
  GtkWidget *hbox_pages = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
  ctx->btn_prev = gtk_button_new_with_label("Prev");
  ctx->label_page = gtk_label_new("0 / 0");
  ctx->btn_next = gtk_button_new_with_label("Next");
  gtk_box_pack_start(GTK_BOX(hbox_pages), ctx->btn_prev, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(hbox_pages), ctx->label_page, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(hbox_pages), ctx->btn_next, TRUE, TRUE, 0);

  // 알고리즘 콤보
  ctx->algo_combo = gtk_combo_box_text_new();
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(ctx->algo_combo), "DFS");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(ctx->algo_combo), "HDFS");
  gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(ctx->algo_combo), "DLX");
  gtk_combo_box_set_active(GTK_COMBO_BOX(ctx->algo_combo), 0);

  // Run 버튼
  ctx->btn_run = gtk_button_new_with_label("Run");

  // 배치
  gtk_box_pack_start(GTK_BOX(parent), ctx->grid_widget, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(parent), hbox_pages, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(parent), ctx->algo_combo, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(parent), ctx->btn_run, FALSE, FALSE, 0);

  // 시그널 연결: on_run_clicked, on_prev_clicked, on_next_clicked
  g_signal_connect(ctx->btn_run, "clicked", G_CALLBACK(on_run_clicked), ctx);
  g_signal_connect(ctx->btn_prev, "clicked", G_CALLBACK(on_prev_clicked), ctx);
  g_signal_connect(ctx->btn_next, "clicked", G_CALLBACK(on_next_clicked), ctx);
}

static void init_third_column(AppContext *ctx, GtkBox *parent) {
  // 테스트와 글씨
  GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
  GtkWidget *label = gtk_label_new("Result Table");
  // 보고서용 테스트
  ctx->btn_test = gtk_button_new_with_label("5×Level Test");

  // 결과 표
  // 내부에서 시그널 연결: on_result_selection_changed
  GtkWidget *result_scrolled = create_result_table(ctx);

  // 엑셀 저장용 버튼
  ctx->btn_save = gtk_button_new_with_label("Save");

  // 배치
  gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 40);
  gtk_box_pack_end(GTK_BOX(hbox), ctx->btn_test, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(parent), hbox, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(parent), result_scrolled, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(parent), ctx->btn_save, FALSE, FALSE, 0);

  // 시그널 연결: on_save_clicked, on_treeview_right_click, on_test_clicked
  // 내부에서 시그널 연결: on_delete_row
  g_signal_connect(ctx->btn_save, "clicked", G_CALLBACK(on_save_clicked), ctx);
  g_signal_connect(ctx->result_table, "button-press-event",
                   G_CALLBACK(on_treeview_right_click), ctx);
  g_signal_connect(ctx->btn_test, "clicked", G_CALLBACK(on_test_clicked), ctx);
}

void create_main_window(GtkApplication *app) {
  GtkWidget *window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Sudoku Solver");       // 제목
  gtk_window_set_default_size(GTK_WINDOW(window), 900, 400);       // 창 크기
  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER); // 위치
  gtk_container_set_border_width(GTK_CONTAINER(window), 30);       // padding

  // css
  GtkCssProvider *provider = gtk_css_provider_new();
  gtk_css_provider_load_from_path(provider, "res/style.css", NULL);
  gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                            GTK_STYLE_PROVIDER(provider),
                                            GTK_STYLE_PROVIDER_PRIORITY_USER);
  g_object_unref(provider);

  // ctx 설정
  AppContext *ctx = app_context_new(GTK_WINDOW(window));

  // 전체 grid
  GtkWidget *grid = gtk_grid_new();
  gtk_grid_set_column_spacing(GTK_GRID(grid), 24);
  gtk_container_add(GTK_CONTAINER(window), grid);

  // 1, 2, 3 열
  GtkWidget *vbox1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
  init_first_column(ctx, GTK_BOX(vbox1));
  GtkWidget *vbox2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
  init_second_column(ctx, GTK_BOX(vbox2));
  GtkWidget *vbox3 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 12);
  init_third_column(ctx, GTK_BOX(vbox3));

  // 배치 및 조정
  gtk_widget_set_hexpand(vbox3, TRUE);
  gtk_grid_attach(GTK_GRID(grid), vbox1, 0, 0, 1, 1);
  gtk_grid_attach(GTK_GRID(grid), vbox2, 1, 0, 2, 1);
  gtk_grid_attach(GTK_GRID(grid), vbox3, 3, 0, 10, 1);

  gtk_widget_show_all(window);
}
