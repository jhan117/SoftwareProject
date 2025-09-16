#include <stdio.h>
#include "utils.h"

void printBoardSimple(int board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}

void printBoard(int board[SIZE][SIZE], int solution[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                // 노란색
                printf("\033[33m%d \033[0m", solution[i][j]);
            }
            else {
                printf("%d ", board[i][j]);
            }
        }
        printf("\n");
    }
}