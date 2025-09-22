#include "gui/gui_grid.h"
#include <gtk/gtk.h>

void create_grid(AppContext *ctx) {
  GtkWidget *grid = gtk_grid_new();
  gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
  gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);

  for (int row = 0; row < SIZE; row++) {
    for (int col = 0; col < SIZE; col++) {
      GtkWidget *frame = gtk_frame_new(NULL);
      gtk_frame_set_shadow_type(GTK_FRAME(frame), GTK_SHADOW_NONE);

      gtk_widget_set_hexpand(frame, TRUE);
      gtk_widget_set_vexpand(frame, TRUE);
      gtk_widget_set_halign(frame, GTK_ALIGN_FILL);
      gtk_widget_set_valign(frame, GTK_ALIGN_FILL);

      GtkWidget *label = gtk_label_new("");
      gtk_container_add(GTK_CONTAINER(frame), label);
      gtk_widget_set_hexpand(label, TRUE);
      gtk_widget_set_vexpand(label, TRUE);
      gtk_widget_set_halign(label, GTK_ALIGN_FILL);
      gtk_widget_set_valign(label, GTK_ALIGN_FILL);

      GtkStyleContext *style = gtk_widget_get_style_context(frame);
      gtk_style_context_add_class(style, "sudoku-cell");

      if (col % 3 == 0)
        gtk_style_context_add_class(style, "sudoku-cell-left");
      if (row % 3 == 0)
        gtk_style_context_add_class(style, "sudoku-cell-top");
      if (row == SIZE - 1)
        gtk_style_context_add_class(style, "sudoku-cell-bottom");
      if (col == SIZE - 1)
        gtk_style_context_add_class(style, "sudoku-cell-right");

      gtk_grid_attach(GTK_GRID(grid), frame, col, row, 1, 1);
      ctx->cell_labels[row][col] = label;
    }
  }
  ctx->grid_widget = grid;
}

static void set_label_text(GtkLabel *label, int value) {
  gchar buf[4];
  if (value != 0)
    g_snprintf(buf, sizeof(buf), "%d", value);
  else
    buf[0] = '\0';
  gtk_label_set_text(GTK_LABEL(label), buf);
}

static void update_cell_style(AppContext *ctx, GtkWidget *label,
                              SudokuSolutions *sols, int row, int col) {
  GtkWidget *frame = gtk_widget_get_parent(label);

  GtkStyleContext *style = gtk_widget_get_style_context(frame);
  gtk_style_context_remove_class(style, "initial-cell");
  gtk_style_context_remove_class(style, "solution-cell");

  const char *cls =
      (!sols)
          ? (ctx->cur_board[row][col] != 0 ? "initial-cell" : NULL)
          : (ctx->cur_board[row][col] == 0 ? "solution-cell" : "initial-cell");

  if (cls)
    gtk_style_context_add_class(style, cls);
}

void display_solution(AppContext *ctx, SudokuSolutions *sols, int idx) {
  for (int row = 0; row < SIZE; row++) {
    for (int col = 0; col < SIZE; col++) {
      GtkWidget *label = ctx->cell_labels[row][col];

      // 값 설정
      int value = sols ? sols->boards[idx][row][col] : ctx->cur_board[row][col];
      set_label_text(GTK_LABEL(label), value);
      update_cell_style(ctx, label, sols, row, col);
    }
  }
}
