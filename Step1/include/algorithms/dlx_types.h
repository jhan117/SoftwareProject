#pragma once

#include "common.h"

#define MAX_COLS 9 * 9 * 4

typedef struct Column Column;
typedef struct Node Node;

struct Node {
  Node *left, *right, *up, *down;
  Column *col;
  int row_id; // 10 * 10 * r + 10 * c + num
};

struct Column {
  Node head;
  int size;
  Column *prev, *next;
  int index; // 열 번호
};

typedef struct {
  Column header;
  Column *columns;
  int col_count;
} DLX;