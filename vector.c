/*
vec - len,cap,[data]
push: (cap=len?: realloc); ptr[len]=new
pop: (len=0?: err); --len
dealloc: free
get = ptr[i]
*/

#include <stdio.h>
#include <stdlib.h>

#define _VEC_BIAS (sizeof(size_t)*2)
#define _valid_len(ptr) ((size_t*)ptr)[-2]
#define len(ptr) (ptr == NULL ? 0 : _valid_len(ptr))
#define _valid_cap(ptr) ((size_t*)ptr)[-1]
#define cap(ptr) (ptr == NULL ? 0 : _valid_cap(ptr))

void* _grow(void* given_ptr, size_t item_size, void** result) {
    size_t capacity;
    if (given_ptr == NULL) {
        capacity = 8;
    } else if (len(given_ptr) == cap(given_ptr)) {
        size_t GROWTH_FACTOR = 2;
        capacity = cap(given_ptr)*GROWTH_FACTOR;
        if (capacity / GROWTH_FACTOR != cap(given_ptr)) {
            // Overflow
            return NULL;
        }
    } else {
        // Everything is fine for now, no need to allocate
        return given_ptr;
    }
    size_t data_size = item_size * capacity;
    if (data_size / item_size != capacity) {
        // Overflow
        return NULL;
    }
    size_t total_size = data_size + _VEC_BIAS;
    if (total_size <= data_size) {
        // Overflow
        return NULL;
    }
    if (given_ptr != NULL) {
        given_ptr -= _VEC_BIAS;
    }
    void* alloc_ptr = realloc(given_ptr, total_size);
    if (alloc_ptr != NULL) {
        alloc_ptr += _VEC_BIAS;
        if (given_ptr == NULL) {
            _valid_len(alloc_ptr) = 0;
        }
        _valid_cap(alloc_ptr) = capacity;
        *result = alloc_ptr;
    }
    return alloc_ptr;
}
void dealloc_vec(void* ptr) {
    if (ptr != NULL) free(ptr - sizeof(size_t) - sizeof(size_t));
}

#define push(ptr, value) (_grow(ptr, sizeof(*ptr), (void**)&ptr) == NULL ? NULL : ((ptr[_valid_len(ptr)++] = value), ptr))
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
