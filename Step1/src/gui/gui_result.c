#include "gui/gui_result.h"
#include "gui/gui_callbacks_result.h"
#include <gtk/gtk.h>

GtkWidget *create_result_table(AppContext *ctx) {
  ctx->result_store =
      gtk_list_store_new(8,
                         G_TYPE_INT,    // Puzzle #
                         G_TYPE_STRING, // Algorithm
                         G_TYPE_STRING, // Difficulty
                         G_TYPE_INT,    // NumSolutions
                         G_TYPE_INT,    // Avg Calls / Nodes
                         G_TYPE_DOUBLE, // Avg Time (ms)
                         G_TYPE_STRING, // Initial board (hidden)
                         G_TYPE_STRING  // Solution board (hidden)
      );

  GtkWidget *treeview =
      gtk_tree_view_new_with_model(GTK_TREE_MODEL(ctx->result_store));
  ctx->result_table = GTK_TREE_VIEW(treeview);

  const char *col_names[] = {"#",       "Algo",      "Diff",
                             "Num Sol", "Avg Calls", "Time(ms)"};

  for (int i = 0; i < 6; i++) {
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes(
        col_names[i], renderer, "text", i, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
  }

  GtkTreeSelection *selection =
      gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
  gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);

  // 시그널: on_result_selection_changed
  g_signal_connect(selection, "changed",
                   G_CALLBACK(on_result_selection_changed), ctx);

  GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
  gtk_container_add(GTK_CONTAINER(scrolled), treeview);

  return scrolled;
}
