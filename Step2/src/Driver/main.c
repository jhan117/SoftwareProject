#include <memory.h>

void main(void) {
    int a, b, c, d;

    a = my_alloc(10);
    b = my_alloc(20);
    c = my_alloc(10);
    my_free(a);
    a = my_alloc(30);
    my_free(c);
    d = my_alloc(20);
    my_free(b);
}