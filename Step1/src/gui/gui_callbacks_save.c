#include "gui/gui_callbacks_save.h"

#include <stdio.h>

static void save_results_to_csv(AppContext *ctx, const char *filename) {
  FILE *fp = fopen(filename, "w");
  if (!fp) {
    g_warning("Cannot open file for writing: %s", filename);
    return;
  }

  // 헤더
  fprintf(
      fp,
      "Puzzle,Algorithm,Difficulty,NumSolutions,AvgCallsOrNodes,AvgTime(ms)\n");

  GtkTreeIter iter;
  gboolean valid;
  valid =
      gtk_tree_model_get_iter_first(GTK_TREE_MODEL(ctx->result_store), &iter);
  while (valid) {
    int puzzle, num_solutions, avg_calls;
    gchar *algo, *diff;
    double avg_time;

    gtk_tree_model_get(GTK_TREE_MODEL(ctx->result_store), &iter, 0, &puzzle, 1,
                       &algo, 2, &diff, 3, &num_solutions, 4, &avg_calls, 5,
                       &avg_time, -1);

    fprintf(fp, "%d,%s,%s,%d,%d,%.3f\n", puzzle, algo, diff, num_solutions,
            avg_calls, avg_time);

    g_free(algo);
    g_free(diff);

    valid = gtk_tree_model_iter_next(GTK_TREE_MODEL(ctx->result_store), &iter);
  }

  fclose(fp);
}

void on_save_clicked(GtkButton *button, gpointer user_data) {
  AppContext *ctx = user_data;

  GtkWidget *dialog = gtk_file_chooser_dialog_new(
      "Save CSV", GTK_WINDOW(ctx->window), GTK_FILE_CHOOSER_ACTION_SAVE,
      "_Cancel", GTK_RESPONSE_CANCEL, "_Save", GTK_RESPONSE_ACCEPT, NULL);
  gtk_window_set_resizable(GTK_WINDOW(dialog), FALSE);

  gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog),
                                                 TRUE);
  gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), "results.csv");

  if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
    char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
    save_results_to_csv(ctx, filename);
    g_free(filename);
  }

  gtk_widget_destroy(dialog);
}