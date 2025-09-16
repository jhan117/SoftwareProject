#include <stdio.h>
#include "hdfs.h"

void initBoard(SudokuBoard* board, int input[SIZE][SIZE]) {
    board->empty_count = 0;

    int rowUsed[SIZE][SIZE] = { 0 };
    int colUsed[SIZE][SIZE] = { 0 };
    int boxUsed[SIZE][SIZE] = { 0 };

    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++) {
            int val = board->cells[i][j].value = input[i][j];

            if (val) {
                int num = val - 1;
                rowUsed[i][num] = 1;
                colUsed[j][num] = 1;
                boxUsed[(i / 3) * 3 + (j / 3)][num] = 1;
            } else {
                board->empty_count++;
            }

            for (int i = 0; i < SIZE; i++)
                for (int j = 0; j < SIZE; j++) {
                    if (board->cells[i][j].value) {
                        for (int n = 0; n < SIZE; n++)
                            board->cells[i][j].candidates[n] = 0;
                        continue;
                    }
                    for (int n = 0; n < SIZE; n++)
                        board->cells[i][j].candidates[n] =
                        !(rowUsed[i][n] || colUsed[j][n] || boxUsed[(i / 3) * 3 + (j / 3)][n]);
                }
        }
}

int isValidHDFS(SudokuBoard* board, int row, int col, int num) {
    for (int i = 0; i < SIZE; i++) {
        if (board->cells[row][i].value == num) return 0;
        if (board->cells[i][col].value == num) return 0;
    }
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board->cells[startRow + i][startCol + j].value == num) return 0;
    return 1;
}

Cell* MRV(SudokuBoard* board, int* out_row, int* out_col) {
    int min_count = SIZE + 1;
    Cell* selected = NULL;

    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (board->cells[row][col].value == 0) {
                int candidate_count = 0;
                for (int n = 0; n < SIZE; n++)
                    if (board->cells[row][col].candidates[n])
                        candidate_count++;
                if (candidate_count < min_count) {
                    min_count = candidate_count;
                    selected = &board->cells[row][col];
                    *out_row = row;
                    *out_col = col;
                }
            }
        }
    }
    return selected;
}

int removeCandidates(SudokuBoard* board, int row, int col, int num, CandidateChange stack[]) {
    int n = num - 1;
    int sr = row - row % 3;
    int sc = col - col % 3;
    int top = 0;

    for (int i = 0; i < SIZE; i++) {
        if (board->cells[row][i].value == 0) {
            stack[top++] = (CandidateChange){ row, i, num, board->cells[row][i].candidates[n] };
            board->cells[row][i].candidates[n] = 0;
        }
        if (board->cells[i][col].value == 0) {
            stack[top++] = (CandidateChange){ i, col, num, board->cells[i][col].candidates[n] };
            board->cells[i][col].candidates[n] = 0;
        }
    }

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) {
            int r = sr + i;
            int c = sc + j;
            if (board->cells[r][c].value == 0) {
                stack[top++] = (CandidateChange){ r, c, num, board->cells[r][c].candidates[n] };
                board->cells[r][c].candidates[n] = 0;
            }
        }
    return top;
}

void restoreCandidates(SudokuBoard* board, CandidateChange stack[], int top) {
    while (top > 0) {
        CandidateChange ch = stack[--top];
        board->cells[ch.row][ch.col].candidates[ch.num - 1] = ch.prev;
    }
}

int solveSudokuHDFS(SudokuBoard* board, SudokuSolutions* sols) {
    if (board->empty_count == 0) {
        if (sols->count < MAX_SOLUTIONS) {
            for (int i = 0; i < SIZE; i++)
                for (int j = 0; j < SIZE; j++)
                    sols->boards[sols->count][i][j] = board->cells[i][j].value;
            sols->count++;
        }
        return 1;
    }

    int row, col;
    Cell* cell = MRV(board, &row, &col);
    if (!cell) return 0;

    CandidateChange stack[SIZE * SIZE * SIZE];
    int top;

    for (int n = 0; n < SIZE; n++) {
        if (!cell->candidates[n]) continue;
        int num = n + 1;
        if (!isValidHDFS(board, row, col, num)) continue;

        cell->value = num;
        board->empty_count--;

        top = removeCandidates(board, row, col, num, stack);
        solveSudokuHDFS(board, sols);
        restoreCandidates(board, stack, top);

        cell->value = 0;
        board->empty_count++;
    }

    return 0;
}