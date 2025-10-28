#include "memory.h"
#include <stdio.h>

void print_memory_info(MemoryInfo* info, int count) {
    for (int i = 0; i < count; i++) {
        printf("Block %d: start=%d, size=%d, next=%d, used=%d, is_best=%d\n",
            i, info[i].start, info[i].size, info[i].next, info[i].used, info[i].is_best);
    }
}

int main() {
    int a[2], b[2], c[2], d[2];
    MemoryInfo infos[108 * 2];

    printf("First initialization\n");
    init_memory(2, 108);

    memcpy(a, my_alloc(20), 8);
    memcpy(b, my_alloc(30), 8);
    memcpy(c, my_alloc(15), 8);

    printf("Allocation: First-Fit addr=%d, Best-Fit addr=%d\n", a[0], a[1]);
    printf("Allocation: First-Fit addr=%d, Best-Fit addr=%d\n", b[0], b[1]);
    printf("Allocation: First-Fit addr=%d, Best-Fit addr=%d\n", c[0], c[1]);
   
    int blockCount = get_memory_infos(infos);
    print_memory_info(infos, blockCount);

    my_free(b);
    my_free(c);
    memcpy(d, my_alloc(20), 8);

    blockCount = get_memory_infos(infos);
    print_memory_info(infos, blockCount);

    return 0;
}