#include "vector.c"

int main(void) {
#define printval(format, value) printf(#value " = " format "\n", value)
#define section(line) printf("### %s ###\n", line)
    int* v = EMPTY_VEC;
    section("Testing basic functions");
    push(v, 5);
    push(v, 15);
    printval("%d", v[0]);
    printval("%d", v[1]);
    printval("%zu", cap(v));
    printval("%zu", len(v));
    printval("%d", pop(v));
    printval("%d", pop(v));
    printval("%zu", cap(v));
    printval("%zu", len(v));
    section("Testing realloc");
    while (cap(v) == _VEC_INITIAL_CAPACITY) {
        push(v, 1);
        printval("%zu", len(v));
    }
    printval("%zu", cap(v));
    for (int i = 0; i < len(v); ++i) {
        printf("v[%d] = %d\n", i, v[i]);
    }
    dealloc_vec(v);
    return 0;
}
