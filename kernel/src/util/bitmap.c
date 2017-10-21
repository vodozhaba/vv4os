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
#define FULL_WORD(b) ((b) ? ~((size_t) 0) : 0)
#define REAL_IX(m, x) ((x) - (m)->dead_zone)
#define WORD_AT(m, x) (((size_t*) (m)->start)[REAL_IX((m), (x))])
#define GET_BIT(m, x) (GetBit(WORD_AT(m, (x) / BITS_PER_WORD), (x) % BITS_PER_WORD))
#define SET_BIT(m, x, b) (SetBit(&WORD_AT((m), (x) / BITS_PER_WORD), (x) % BITS_PER_WORD, (b)))

static size_t FindInBitmap(Bitmap* bitmap, size_t len) {
    assert(bitmap);
    size_t found = 0;
    // TODO: add word compare optmization
    for(size_t i = bitmap->dead_zone; i < bitmap->len; i++) {
        if(GET_BIT(bitmap, i)) {
            found = 0;
            continue;
        }
        found++;
        if(found < len) {
            continue;
        }
        return i - found + 1;
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
    // TODO: add word set optmization
    for(size_t i = first; i < first + len; i++) {
        SET_BIT(bitmap, i, allocated);
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
