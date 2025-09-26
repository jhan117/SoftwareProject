#pragma once

#include "common.h"
#include "solution_types.h"
#include <gtk/gtk.h>

typedef enum { OK = 0, FORMAT_ERROR, RULE_ERROR } Status;

typedef struct {
  // sudoku_grid
  GtkWidget *grid_widget;
  GtkWidget *cell_labels[SIZE][SIZE];

  // buttons
  GtkWidget *btn_gen;
  GtkWidget *btn_prev;
  GtkWidget *btn_next;
  GtkWidget *btn_run;
  GtkWidget *btn_test;
  GtkWidget *btn_save;
  GtkWidget *label_page;
  // input and combobox
  GtkWidget *input_textview;
  GtkWidget *algo_combo;
  GtkWidget *level_combo;

  // status
  SudokuSolutions *sols;
  int current_idx;

  // current board
  int cur_board[SIZE][SIZE];

  // result data
  GtkTreeView *result_table;
  GtkListStore *result_store;
  int result_store_count;

  int difficulty_level;

  GtkWindow *window;
} AppContext;

void reset_context(AppContext *ctx);