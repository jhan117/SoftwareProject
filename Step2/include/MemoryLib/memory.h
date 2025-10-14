#pragma once

#define SIZE 108

typedef struct {
    int size; // 블록 크기
    int next; // 다음 빈 블록 인덱스, 없으면 -1
} Header;

int my_alloc(int request_size);
void my_free(int addr);