// File:       util/bitwise.h
// Author:     vodozhaba
// Created on: Oct 21, 2017
// Purpose:    Bitwise operations.

#include <stdbool.h>
#include <stddef.h>

static inline bool GetBit(size_t word, size_t b) {
    return (word >> b) & 1;
}

static inline void SetBit(size_t* word, size_t b, bool value) {
    if(value) {
        *word |= ((size_t) 1 << b);
    } else {
        *word &= ~((size_t) 1 << b);
    }
}
