#include "utils/solver.h"

int average_metric(const char *algorithm, SudokuSolutions *sols) {
  int n = sols->count;
  int sum_calls = 0;

  for (int i = 0; i < n; i++) {
    if (strcmp(algorithm, "DFS") == 0) {
      sum_calls += sols->dfs_calls[i];
    } else if (strcmp(algorithm, "HDFS") == 0) {
      sum_calls += sols->hdfs_calls[i];
    } else if (strcmp(algorithm, "DLX") == 0) {
      sum_calls += sols->dlx_node_visits[i];
    }
  }

  return n ? sum_calls / n : 0;
}

SudokuSolutions *run_algorithm(int board[SIZE][SIZE], int algo) {
  SudokuSolutions *sols = g_malloc0(sizeof(SudokuSolutions));

  int calls = 0;
  switch (algo) {
  case 0:
    solveSudokuDFS(board, sols, &calls);
    break;
  case 1: {
    SudokuBoard sBoard;
    initBoard(&sBoard, board);
    solveSudokuHDFS(&sBoard, sols, &calls);
    break;
  }
  case 2: {
    DLX dlx;
    initDLX(&dlx);
    int solution[SIZE * SIZE];
    boardToDLX(&dlx, board);
    solveSudokuDLX(&dlx, solution, 0, sols, &calls);
    break;
  }
  default:
    g_free(sols);
    return NULL;
  }

  return sols;
}