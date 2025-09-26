#pragma once

#include "app_context.h"

void on_result_selection_changed(GtkTreeSelection *selection,
                                 gpointer user_data);
gboolean on_treeview_right_click(GtkWidget *tree_view, GdkEventButton *event,
                                 gpointer user_data);