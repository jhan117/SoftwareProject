#pragma once

#include "algorithms/dfs.h"
#include "algorithms/dlx.h"
#include "algorithms/hdfs.h"
#include "solution_types.h"
#include <glib.h>
#include <stdio.h>

int average_metric(const char *algorithm, SudokuSolutions *sols);
SudokuSolutions *run_algorithm(int board[SIZE][SIZE], int algo);