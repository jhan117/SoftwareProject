#pragma once

#define MAX_MEM_SIZE 512

typedef struct {
    int size; // 블록 크기
    int next; // 다음 빈 블록 인덱스, 없으면 -1
} Header;

typedef struct {
    char memory[MAX_MEM_SIZE];
    int free_head;
    int is_best; // best-fit 여부
    int size;
} MemoryContext;

typedef struct {
    int start;
    int size;
    int next;
    int used;   // 할당중인지
    int is_best; // best-fit 여부
} MemoryInfo;

void init_memory(int num, int user_size);
int* my_alloc(int request_size);
void my_free(int* addrs);

int get_memory_infos(MemoryInfo* info);