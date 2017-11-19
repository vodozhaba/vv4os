// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       util/bitmap.c
// Author:     vodozhaba
// Created on: Oct 20, 2017
// Purpose:    One "library" for all bitmaps in the project.

#include "bitmap.h"
#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include "util/bitwise.h"

#define BITS_PER_WORD (sizeof(size_t) * CHAR_BIT)

static size_t FindInBitmap(Bitmap* bitmap, size_t len) {
    assert(bitmap);
    size_t* words = (size_t*) bitmap->start;
    for(size_t i = bitmap->dead_zone, found = 0; i < bitmap->len; i++) {
        if(words[(i - bitmap->dead_zone) / BITS_PER_WORD] & (1 << ((i - bitmap->dead_zone) % BITS_PER_WORD))) {
            found = 0;
            continue;
        }
        found++;
        if(found >= len) {
            return i - len + 1;
        }
    }
    return BITMAP_INVALID_WORD_IX;
}

void InitBitmap(Bitmap* bitmap, bool allocated) {
    assert(bitmap);
    MarkInBitmap(bitmap, bitmap->dead_zone, bitmap->len - bitmap->dead_zone, allocated);
}

void MarkInBitmap(Bitmap* bitmap, size_t first, size_t len, bool allocated) {
    assert(bitmap);
    assert(first >= bitmap->dead_zone);
    size_t* words = (size_t*) bitmap->start;
    for(size_t i = first; i < first + len; i++) {
        SetBit(&words[(i - bitmap->dead_zone) / BITS_PER_WORD], (i - bitmap->dead_zone) % BITS_PER_WORD, allocated);
    }
}

size_t AllocateInBitmap(Bitmap* bitmap, size_t len) {
    assert(bitmap);
    size_t first = FindInBitmap(bitmap, len);
    MarkInBitmap(bitmap, first, len, true);
    return first;
}

void FreeInBitmap(Bitmap* bitmap, size_t first, size_t len) {
    assert(bitmap);
    MarkInBitmap(bitmap, first, len, false);
}
