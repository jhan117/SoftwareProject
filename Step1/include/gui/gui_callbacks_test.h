#pragma once

#include "app_context.h"

typedef struct {
  int level;
  int algo;
  int repeat;                         // test 반복 개수
  int saved_boards[3][5][SIZE][SIZE]; // 레벨별로 저장해두기
  AppContext *ctx;
} TaskData;

void on_test_clicked(GtkButton *button, gpointer user_data);