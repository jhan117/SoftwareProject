#include "gui/gui.h"

static void activate(GtkApplication *app, gpointer user_data) {
  create_main_window(app);
}

int main(int argc, char **argv) {
  srand(time(NULL)); // 최상단에서 한번만 호출

  GtkApplication *app =
      gtk_application_new("com.kaye.sudoku", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

  int status = g_application_run(G_APPLICATION(app), argc, argv);

  g_object_unref(app);
  return status;
}
