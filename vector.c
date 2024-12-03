/*
vec - len,cap,[data]
push: (cap=len?: realloc); ptr[len]=new
pop: (len=0?: err); --len
dealloc: free
get = ptr[i]
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define _VEC_BIAS (sizeof(size_t)*2)
#define _valid_len(ptr) ((size_t*)ptr)[-2]
#define len(ptr) (ptr == NULL ? 0 : _valid_len(ptr))
#define _valid_cap(ptr) ((size_t*)ptr)[-1]
#define cap(ptr) (ptr == NULL ? 0 : _valid_cap(ptr))

// Should return new ptr; grows by at least 1
void* _try_grow(void* ptr, size_t item_size) {
    size_t item_count;
    if (ptr == NULL) {
        item_count = 0;
    } else if (len(ptr) == cap(ptr)) {
        item_count = cap(ptr)*2;
        if (item_count / 2 != cap(ptr)) {
            // Overflow
            return NULL;
        }
    } else {
        return ptr;
    }
    size_t total_size = item_size * item_count;
    if (total_size / item_size != item_count) {
        // Overflow
        return NULL;
    } else {
        // No overflow
        _valid_len(ptr)++;
        _valid_cap(ptr) = item_count;
        return realloc(ptr - _VEC_BIAS, total_size) + _VEC_BIAS;
    }
}
void dealloc_vec(void* ptr) {
    if (ptr != NULL) free(ptr - sizeof(size_t) - sizeof(size_t));
}

#define push(ptr, value) ((ptr = _try_grow(ptr, sizeof(*ptr))) == NULL ? NULL : (ptr[len(ptr)] = value, ptr))
#define pop(ptr) ptr[(_valid_len(ptr)--) - 1]
#define vec() NULL

int main(void) {
    int* a = vec();
    push(a, 5);
    push(a, 15);
    printf("%d\n", a[0]);
    printf("%d\n", a[1]);
    printf("%zu\n", cap(a));
    printf("%zu\n", len(a));
    printf("%d\n", pop(a));
    printf("%d\n", pop(a));
    printf("%zu\n", cap(a));
    printf("%zu\n", len(a));
    dealloc_vec(a);
    return 0;
}
