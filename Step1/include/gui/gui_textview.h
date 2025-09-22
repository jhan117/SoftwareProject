#pragma once

#include "app_context.h"
#include <gtk/gtk.h>

Status board_from_textview(GtkTextView *text_view, int board[SIZE][SIZE]);
void textview_from_board(GtkTextView *text_view, int board[SIZE][SIZE]);
