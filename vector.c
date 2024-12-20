/*
vec - len,cap,[data]
*/

#include <stdio.h>
#include <stdlib.h>

static const size_t _VEC_DATA_BIAS = sizeof(size_t)*2;
static const size_t _VEC_GROWTH_FACTOR = 2;
static const size_t _VEC_INITIAL_CAPACITY = 8;
#define _valid_len(ptr) ((size_t*)ptr)[-2]
#define _valid_cap(ptr) ((size_t*)ptr)[-1]

size_t len(void* ptr) {
    return ptr == NULL ? 0 : _valid_len(ptr);
}

size_t cap(void* ptr) {
    return ptr == NULL ? 0 : _valid_cap(ptr);
}

void* _prepare_for_push(void** result, size_t item_size) {
    void* given_ptr = *result;
    size_t capacity;
    if (given_ptr == NULL) {
        capacity = _VEC_INITIAL_CAPACITY;
    } else if (_valid_len(given_ptr) == _valid_cap(given_ptr)) {
        capacity = _valid_cap(given_ptr)*_VEC_GROWTH_FACTOR;
        if (capacity / _VEC_GROWTH_FACTOR != _valid_cap(given_ptr)) {
            // Overflow
            return NULL;
        }
    } else {
        // Everything is fine for now, no need to reallocate
        return given_ptr;
    }
    size_t data_size = item_size * capacity;
    if (data_size / item_size != capacity) {
        // Overflow
        return NULL;
    }
    size_t total_size = data_size + _VEC_DATA_BIAS;
    if (total_size <= data_size) {
        // Overflow
        return NULL;
    }
    if (given_ptr != NULL) {
        given_ptr -= _VEC_DATA_BIAS;
    }
    void* alloc_ptr = realloc(given_ptr, total_size);
    if (alloc_ptr != NULL) {
        alloc_ptr += _VEC_DATA_BIAS;
        if (given_ptr == NULL) {
            _valid_len(alloc_ptr) = 0;
        }
        _valid_cap(alloc_ptr) = capacity;
        *result = alloc_ptr;
    }
    return alloc_ptr;
}

void dealloc_vec(void* ptr) {
    if (ptr != NULL) free(ptr - _VEC_DATA_BIAS);
}

#define push(ptr, value) (_prepare_for_push((void**)&ptr, sizeof(*ptr)) == NULL ? NULL : ((ptr[_valid_len(ptr)++] = value), ptr))
#define pop(ptr) ptr[(_valid_len(ptr)--) - 1]
#define EMPTY_VEC NULL
