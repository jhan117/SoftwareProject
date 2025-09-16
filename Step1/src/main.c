#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "generator.h"
#include "dfs.h"
#include "dlx.h"
#include "hdfs.h"

int main() {
    srand(time(NULL));
    int board[SIZE][SIZE];

    int choice;
    printf("1: 스도쿠 직접 입력\n2: 난이도별 자동 생성\n선택: ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("스도쿠를 입력하세요 (0은 빈칸):\n");
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                scanf("%d", &board[i][j]);
    } else if (choice == 2) {
        printf("난이도 선택 (1: 쉬움, 2: 보통, 3: 어려움): ");
        int level;
        scanf("%d", &level);
        generateRandomSudoku(board, level);
        printf("\n문제\n");
        printBoardSimple(board);
    }
    else {
        printf("잘못된 선택입니다.\n");
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

    // 시간 넘 미미해서 횟수 비율로 시간 변경할 것
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;

    if (sols.count == 0) {
        printf("해답이 없습니다.\n");
    }
    else {
        printf("찾은 해답 수: %d\n\n", sols.count);
        for (int k = 0; k < sols.count; k++) {
            printf("해답 %d:\n", k + 1);
            printBoard(board, sols.boards[k]);
        }
        printf("풀이 시간: %.3f ms\n", elapsed_time * 1000.00);
    }

    return 0;
}
