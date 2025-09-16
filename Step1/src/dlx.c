#include <stdio.h>
#include <stdlib.h>

#include "dlx.h"

void initDLX(DLX* dlx) {
    dlx->col_count = MAX_COLS;
    dlx->columns = (Column*)malloc(sizeof(Column) * MAX_COLS);
    if (dlx->columns == NULL) {
        fprintf(stderr, "메모리 할당 실패: dlx->columns\n");
        exit(EXIT_FAILURE);
    }

    dlx->header.next = dlx->header.prev = &dlx->header;

    for (int i = 0; i < MAX_COLS; i++) {
        Column* c = &dlx->columns[i];
        c->index = i;
        c->size = 0;

        // 어째서인지 prev를 먼저 하면 문제 생김...
        c->next = &dlx->header;
        c->prev = dlx->header.prev;
        dlx->header.prev->next = c;
        dlx->header.prev = c;

        c->head.up = c->head.down = c->head.left = c->head.right = &c->head;
        c->head.col = c;
        c->head.row_id = -1;
    }
}

void addRow(DLX* dlx, int row_id, int* col_indices, int col_num) {
    Node* rowNodes[4];

    for (int i = 0; i < col_num; i++) {
        int col_idx = col_indices[i];
        Column* c = &dlx->columns[col_idx];
        Node* node = (Node*)malloc(sizeof(Node));
        if (node == NULL) {
            fprintf(stderr, "메모리 할당 실패: node\n");
            exit(EXIT_FAILURE);
        }
        node->col = c;
        node->row_id = row_id;

        // 수직 연결
        node->down = &c->head;
        node->up = c->head.up;
        c->head.up->down = node;
        c->head.up = node;

        c->size++;

        rowNodes[i] = node;
    }

    // 수평 연결
    for (int i = 0; i < col_num; i++) {
        rowNodes[i]->left = rowNodes[(i + 3) % col_num];
        rowNodes[i]->right = rowNodes[(i + 1) % col_num];
    }
}

void boardToDLX(DLX* dlx, int board[SIZE][SIZE]) {
    for (int r = 0; r < SIZE; r++) {
        for (int c = 0; c < SIZE; c++) {
            if (board[r][c] != 0) {
                int num = board[r][c];
                int cols[4] = {
                    r * SIZE + c, // 한 셀에 81개
                    SIZE * SIZE + r * SIZE + (num - 1), // 행에 81개
                    (SIZE * SIZE * 2) + c * SIZE + (num - 1), // 열에 81개
                    (SIZE * SIZE * 3) + ((r / 3) * 3 + (c / 3)) * SIZE + (num - 1) // 3x3 박스에 81개
                };
                addRow(dlx, 100 * r + 10 * c + num, cols, 4);
            }
            else {
                for (int num = 1; num <= SIZE; num++) {
                    int cols[4] = {
                    r * SIZE + c, // 한 셀에 81개
                    SIZE * SIZE + r * SIZE + (num - 1), // 행에 81개
                    (SIZE * SIZE * 2) + c * SIZE + (num - 1), // 열에 81개
                    (SIZE * SIZE * 3) + ((r / 3) * 3 + (c / 3)) * SIZE + (num - 1) // 3x3 박스에 81개
                    };
                    addRow(dlx, 100 * r + 10 * c + num, cols, 4);
                }
            }
        }
    }
}

Column* chooseColumnWithMinSize(DLX* dlx) {
    Column* header = &dlx->header;
    Column* minCol = header->next;

    for (Column* c = header->next->next; c != header; c = c->next) {
        if (c->size < minCol->size) {
            minCol = c;
        }
    }

    return minCol;
}

void decodeSolution(int solution[], int k, int board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            board[i][j] = 0;

    for (int idx = 0; idx < k; idx++) {
        int row_id = solution[idx];
        int r = row_id / 100;
        int c = (row_id / 10) % 10;
        int num = row_id % 10;

        if (r >= 0 && r < SIZE && c >= 0 && c < SIZE)
            board[r][c] = num;
    }
}

void cover(Column* c) {
    c->prev->next = c->next;
    c->next->prev = c->prev;

    for (Node* i = c->head.down; i != &c->head; i = i->down) {
        for (Node* j = i->right; j != i; j = j->right) {
            j->up->down = j->down;
            j->down->up = j->up;

            j->col->size--;
        }
    }
}

void uncover(Column* c) {
    for (Node* i = c->head.up; i != &c->head; i = i->up) {
        for (Node* j = i->left; j != i; j = j->left) {
            j->up->down = j;
            j->down->up = j;

            j->col->size++;
        }
    }

    c->prev->next = c;
    c->next->prev = c;
}


void solveSudokuDLX(DLX* dlx, int solution[], int k, SudokuSolutions* sols) {
    Column* header = &(dlx->header);

    if (header->next == header) {
        if (sols->count < MAX_SOLUTIONS) {
            decodeSolution(solution, k, sols->boards[sols->count]);
            sols->count++;
        }
        return;
    }

    Column* c = chooseColumnWithMinSize(dlx);
    cover(c);

    for (Node* r = c->head.down; r != &(c->head); r = r->down) {
        solution[k] = r->row_id;

        for (Node* j = r->right; j != r; j = j->right)
            cover(j->col);

        solveSudokuDLX(dlx, solution, k + 1, sols);

        for (Node* j = r->left; j != r; j = j->left)
            uncover(j->col);
    }

    uncover(c);
}   
