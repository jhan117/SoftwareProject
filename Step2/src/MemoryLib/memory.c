#include "MemoryLib/memory.h"

#include <stdio.h>

int free_head = -1;
char memory[SIZE];
int is_init = 0;

static void init_memory() {
    free_head = 0;
    // 배열 내 인덱스로 헤더 접근
    Header* block = (Header*)&memory[free_head];
    block->size = SIZE - sizeof(Header);
    block->next = -1;
    is_init = 1;
    printf("[Init] Memory initialized, free_head=%d, size=%d\n", free_head, block->size);
}

int my_alloc(int request_size) {
    if (!is_init) init_memory();
    if (free_head == -1) {
        printf("[Alloc] No free block available\n");
        return -1;
    }

    int prev_idx = -1;
    int curr_idx = free_head;

    while (curr_idx != -1) {
        Header* curr_block = (Header*)&memory[curr_idx];
        if (curr_block->size >= request_size) {
            // 분할 가능한 경우
            if ((curr_block->size - request_size) > sizeof(Header)) {
                int allocated_size = sizeof(Header) + request_size;
                int remaining_idx = curr_idx + allocated_size;

                Header* remaining_block = (Header*)&memory[remaining_idx];
                remaining_block->size = curr_block->size - allocated_size;
                remaining_block->next = curr_block->next;

                if (prev_idx == -1) free_head = remaining_idx;
                else {
                    Header* prev_block = (Header*)&memory[prev_idx];
                    prev_block->next = remaining_idx;
                }
                curr_block->size = request_size;
                printf("[Alloc] Block split: allocated_idx=%d, size=%d, remaining_idx=%d, size=%d\n",
                    curr_idx, request_size, remaining_idx, remaining_block->size);
            }
            // 분할 불가능한 경우 -> 전체 할당
            else {
                if (prev_idx == -1) free_head = curr_block->next;
                else {
                    Header* prev_block = (Header*)&memory[prev_idx];
                    prev_block->next = curr_block->next;
                }
                printf("[Alloc] Block fully allocated: idx=%d, size=%d\n", curr_idx, curr_block->size);
            }
            curr_block->next = -1;
            return curr_idx + sizeof(Header);
        }
        prev_idx = curr_idx;
        curr_idx = curr_block->next;
    }
    printf("[Alloc] Allocation failed for size=%d\n", request_size);
    return -1;
}

void my_free(int addr) {
    if (!is_init) {
        printf("[Free] Error: memory not initialized\n");
        return;
    }
    if (addr < 0 || addr >= SIZE) {
        printf("[Free] Error: invalid address %d\n", addr);
        return;
    }

    int block_idx = addr - sizeof(Header);
    Header* block = (Header*)&memory[block_idx];

    if (free_head == -1) {
        free_head = block_idx;
        block->next = -1;
        printf("[Free] Free list was empty, added block_idx=%d, size=%d\n", block_idx, block->size);
        return;
    }

    int prev_idx = -1;
    int curr_idx = free_head;
    while (curr_idx != -1) {
        if (curr_idx > block_idx) break;
        if (curr_idx == block_idx) {
            printf("[Free] Error: double free attempt detected at idx=%d\n", block_idx);
            return;
        }
        Header* curr_block = (Header*)&memory[curr_idx];
        prev_idx = curr_idx;
        curr_idx = curr_block->next;
    }

    int next_idx = curr_idx;
    block->next = next_idx;
    if (prev_idx == -1) free_head = block_idx;
    else {
        Header* prev_block = (Header*)&memory[prev_idx];
        prev_block->next = block_idx;
    }

    // 주소 계산: 이전 블록의 인덱스 + 헤더 크기 + 이전 블록 크기
    if (prev_idx != -1) {
        Header* prev_block = (Header*)&memory[prev_idx];
        if (prev_idx + sizeof(Header) + prev_block->size == block_idx) {
            prev_block->size += sizeof(Header) + block->size;
            prev_block->next = block->next;
            block_idx = prev_idx;
            block = prev_block;
            printf("[Free] Merged with previous block: idx=%d, new_size=%d\n", block_idx, block->size);
        }

    }

    if (next_idx != -1) {
        Header* next_block = (Header*)&memory[next_idx];
        if (block_idx + sizeof(Header) + block->size == next_idx) {
            block->size += sizeof(Header) + next_block->size;
            block->next = next_block->next;
            printf("[Free] Merged with next block: idx=%d, new_size=%d\n", block_idx, block->size);
        }
    }
    printf("[Free] Block freed: idx=%d, size=%d\n", block_idx, block->size);
}
