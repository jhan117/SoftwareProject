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
    printf("1: ������ ���� �Է�\n2: ���̵��� �ڵ� ����\n����: ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("������ �Է��ϼ��� (0�� ��ĭ):\n");
        for (int i = 0; i < SIZE; i++)
            for (int j = 0; j < SIZE; j++)
                scanf("%d", &board[i][j]);
    } else if (choice == 2) {
        printf("���̵� ���� (1: ����, 2: ����, 3: �����): ");
        int level;
        scanf("%d", &level);
        generateRandomSudoku(board, level);
        printf("\n����\n");
        printBoardSimple(board);
    }
    else {
        printf("�߸��� �����Դϴ�.\n");
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

    // �ð� �� �̹��ؼ� Ƚ�� ������ �ð� ������ ��
    double elapsed_time = (double)(end - start) / CLOCKS_PER_SEC;

    if (sols.count == 0) {
        printf("�ش��� �����ϴ�.\n");
    }
    else {
        printf("ã�� �ش� ��: %d\n\n", sols.count);
        for (int k = 0; k < sols.count; k++) {
            printf("�ش� %d:\n", k + 1);
            printBoard(board, sols.boards[k]);
        }
        printf("Ǯ�� �ð�: %.3f ms\n", elapsed_time * 1000.00);
    }

    return 0;
}
