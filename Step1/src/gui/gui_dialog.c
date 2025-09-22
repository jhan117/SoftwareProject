#include "gui/gui_dialog.h"
#include <gtk/gtk.h>

static const char *status_to_string(Status status) {
  switch (status) {
  case FORMAT_ERROR:
    return "Invalid format";
  case RULE_ERROR:
    return "Rule violated";
  default:
    return "Unknown error";
  }
}

void show_error_dialog(AppContext *ctx, Status status) {
  const char *msg = status_to_string(status);
  if (!msg)
    return;

  GtkWidget *dialog =
      gtk_message_dialog_new(ctx->window, GTK_DIALOG_DESTROY_WITH_PARENT,
                             GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE, "%s", msg);
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}
