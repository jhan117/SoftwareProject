#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gtk/gtk.h>
#include "utils.h"
#include "generator.h"
#include "dfs.h"
#include "dlx.h"
#include "hdfs.h"

int main() {
    srand(time(NULL));
    int board[SIZE][SIZE];

    int choice;
    printf("1: 사용자 입력\n2: 스도쿠 랜덤 생성\n3: 종료");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("스도쿠를 입력하세요:\n");
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                scanf("%d", &board[i][j]);
    } else if (choice == 2) {
        printf("난이도를 입력하세요: ");
        int level;
        scanf("%d", &level);
        generateRandomSudoku(board, level);
        printf("\n문제\n");
        printBoardSimple(board);
    }
    else {
        printf("종료합니다\n");
    }

    SudokuSolutions sols = { 0 };
    DLX dlx;
    int solution[SIZE * SIZE];
    initDLX(&dlx);
    boardToDLX(&dlx, board);
    SudokuBoard sBoard;
    initBoard(&sBoard, board);

    clock_t start = clock();
    //solveSudokuDFS(board, &sols);
    //solveSudokuDLX(&dlx, solution, 0, &sols);
    solveSudokuHDFS(&sBoard, &sols);
    clock_t end = clock();

    // 나중에 횟수로 변경
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;

    if (sols.count == 0) {
        printf("해답이 없습니다.\n");
    }
    else {
        printf("해답 개수: %d\n\n", sols.count);
        for (int k = 0; k < sols.count; k++) {
            printf("해답:\n", k + 1);
            printBoard(board, sols.boards[k]);
        }
        printf("실행 시간: %.3f ms\n", elapsed_time * 1000.00);
    }

    return 0;
}
