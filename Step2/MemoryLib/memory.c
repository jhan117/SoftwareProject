#include "memory.h"

#include <stdio.h>

static int is_init = 0;
static MemoryContext ctx[2];
static int mem_count = 0;
static int alloc_addr[2];

void init_memory(int num, int user_size) {
    if (num < 1 || num > 2) {
        printf("[Init] Error: invalid initialization number %d\n", num);
        return;
    }

    for (int i = 0; i < num; i++) {
        if (user_size > MAX_MEM_SIZE) user_size = MAX_MEM_SIZE;
        memset(ctx[i].memory, 0, user_size);

        ctx[i].free_head = 0;
        Header* block = (Header*)&ctx[i].memory[ctx[i].free_head];
        block->size = user_size - sizeof(Header);
        block->next = -1;
        ctx[i].is_best = (i == 1);
        ctx[i].size = user_size;
        printf("[Init] Memory initialized, free_head=%d, size=%d\n", ctx[i].free_head, block->size);
    }
    is_init = 1;
    mem_count = num;
}

int* my_alloc(int request_size) {
    for (int i = 0; i < mem_count; i++) {
        const char* alloc_type = ctx[i].is_best ? "Best-Fit" : "First-Fit";

        alloc_addr[i] = -1;
        if (!is_init) {
            printf("[Alloc] Error: memory not initialized\n");
            alloc_addr[i] = -1;
            continue;
        }
        if (ctx[i].free_head == -1) {
            printf("[Alloc] No free block available\n");
            alloc_addr[i] = -1;
            continue;
        }
        if (request_size <= 0 || request_size > ctx[i].size - sizeof(Header)) {
            printf("[Alloc] Error: invalid request size %d\n", request_size);
            alloc_addr[i] = -1;
            continue;
        }

        int prev_idx = -1;
        int free_head = ctx[i].free_head;
        int curr_idx = free_head;

        int best_idx = -1;
        int prev_best_idx = -1;
        if (!ctx[i].is_best) {
            // First-Fit
            while (curr_idx != -1) {
                Header* curr_block = (Header*)&ctx[i].memory[curr_idx];
                if (curr_block->size >= request_size) {
                    prev_best_idx = prev_idx;
                    best_idx = curr_idx;
                    break;
                }
                prev_idx = curr_idx;
                curr_idx = curr_block->next;
            }
        }
        else if (ctx[i].is_best) {
            // best-fit
            while (curr_idx != -1) {
                Header* curr_block = (Header*)&ctx[i].memory[curr_idx];
                if (curr_block->size >= request_size) {
                    if (best_idx != -1) {
                        Header* best_block = (Header*)&ctx[i].memory[best_idx];
                        if (curr_block->size < best_block->size) {
                            prev_best_idx = prev_idx;
                            best_idx = curr_idx;
                        }
                    }
                    else {
                        prev_best_idx = prev_idx;
                        best_idx = curr_idx;
                    }
                }
                prev_idx = curr_idx;
                curr_idx = curr_block->next;
            }
        }
        else {
            alloc_addr[i] = -1;
            continue;
        }
        if (best_idx == -1) {
            printf("[Alloc] [%s] Allocation failed for size=%d\n", alloc_type, request_size);
            alloc_addr[i] = -1;
            continue;
        }

        // 분할 가능한 경우
        Header* curr_block = (Header*)&ctx[i].memory[best_idx];
        if ((curr_block->size - request_size) > sizeof(Header)) {
            int allocated_size = sizeof(Header) + request_size;
            int remaining_idx = best_idx + allocated_size;

            Header* remaining_block = (Header*)&ctx[i].memory[remaining_idx];
            remaining_block->size = curr_block->size - allocated_size;
            remaining_block->next = curr_block->next;

            if (prev_best_idx == -1) free_head = remaining_idx;
            else {
                Header* prev_block = (Header*)&ctx[i].memory[prev_best_idx];
                prev_block->next = remaining_idx;
            }
            curr_block->size = request_size;
            printf("[Alloc] [%s] Block split: allocated_idx=%d, size=%d, remaining_idx=%d, size=%d\n",
                alloc_type, best_idx, request_size, remaining_idx, remaining_block->size);
        }
        // 분할 불가능한 경우 -> 전체 할당
        else {
            if (prev_best_idx == -1) free_head = curr_block->next;
            else {
                Header* prev_block = (Header*)&ctx[i].memory[prev_best_idx];
                prev_block->next = curr_block->next;
            }
            printf("[Alloc] [%s] Block fully allocated: idx=%d, size=%d\n", alloc_type, best_idx, curr_block->size);
        }

        curr_block->next = -1;
        ctx[i].free_head = free_head;
        alloc_addr[i] = best_idx + sizeof(Header);
    }
    return alloc_addr;
}
void my_free(int* addrs) {
    for (int i = 0; i < mem_count; i++) {
        const char* alloc_type = ctx[i].is_best ? "Best-Fit" : "First-Fit";
        if (!is_init) {
            printf("[Free] Error: memory not initialized\n");
            return;
        }
        if (addrs[i] < 0 || addrs[i] >= ctx[i].size) {
            printf("[Free] Error: invalid address %d\n", addrs[i]);
            return;
        }

        int free_head = ctx[i].free_head;

        int block_idx = addrs[i] - sizeof(Header);
        Header* block = (Header*)&ctx[i].memory[block_idx];

        if (free_head == -1) {
            free_head = block_idx;
            block->next = -1;
            printf("[Free] [%s] Free list was empty, added block_idx=%d, size=%d\n", alloc_type, block_idx, block->size);
            ctx[i].free_head = free_head;
            continue;
        }

        int prev_idx = -1;
        int curr_idx = free_head;
        while (curr_idx != -1) {
            if (curr_idx > block_idx) break;
            if (curr_idx == block_idx) {
                printf("[Free] [%s] Error: double free attempt detected at idx=%d\n", alloc_type, block_idx);
                return;
            }
            Header* curr_block = (Header*)&ctx[i].memory[curr_idx];
            prev_idx = curr_idx;
            curr_idx = curr_block->next;
        }

        int next_idx = curr_idx;
        block->next = next_idx;
        if (prev_idx == -1) free_head = block_idx;
        else {
            Header* prev_block = (Header*)&ctx[i].memory[prev_idx];
            prev_block->next = block_idx;
        }

        // 주소 계산: 이전 블록의 인덱스 + 헤더 크기 + 이전 블록 크기
        if (prev_idx != -1) {
            Header* prev_block = (Header*)&ctx[i].memory[prev_idx];
            if (prev_idx + sizeof(Header) + prev_block->size == block_idx) {
                prev_block->size += sizeof(Header) + block->size;
                prev_block->next = block->next;
                block_idx = prev_idx;
                block = prev_block;
                printf("[Free] [%s] Merged with previous block: idx=%d, new_size=%d\n", alloc_type, block_idx, block->size);
            }
        }

        if (next_idx != -1) {
            Header* next_block = (Header*)&ctx[i].memory[next_idx];
            if (block_idx + sizeof(Header) + block->size == next_idx) {
                block->size += sizeof(Header) + next_block->size;
                block->next = next_block->next;
                printf("[Free] [%s] Merged with next block: idx=%d, new_size=%d\n", alloc_type, block_idx, block->size);
            }
        }
        ctx[i].free_head = free_head;
        printf("[Free] [%s] Block freed: idx=%d, size=%d\n", alloc_type, block_idx, block->size);
    }
}

int get_memory_infos(MemoryInfo* info) {
    if(!is_init) return 0;
    
    int max_blocks = ctx[0].size * 2;
    int count = 0;
    int i;
    for (i = 0; i < mem_count; i++) {
        Header* memory = (Header*)ctx[i].memory;

        int idx = 0;
        while (idx < ctx[i].size && count < max_blocks) {
            Header* block = (Header*)&ctx[i].memory[idx];

            int used = 1;
            int curr_free = ctx[i].free_head;
            while (curr_free != -1) {
                if (curr_free == idx){
                    used = 0;
                    break;
                }
                Header* f = (Header*)&ctx[i].memory[curr_free];
                curr_free = f->next;
            }

            info[count].start = idx;
            info[count].size = block->size;
            info[count].next = block->next;
            info[count].used = used;
            info[count].is_best = i;

            count++;

            idx += sizeof(Header) + block->size;
        }
    }
    return count;
}