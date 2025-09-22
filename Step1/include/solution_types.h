#pragma once

#include "common.h"

typedef struct {
  int count;
  int boards[MAX_SOLUTIONS][SIZE][SIZE];

  // 분석용
  int dfs_calls[MAX_SOLUTIONS];
  int hdfs_calls[MAX_SOLUTIONS];
  int dlx_node_visits[MAX_SOLUTIONS];
} SudokuSolutions;