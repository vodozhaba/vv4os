// File:       util/bitmap.h
// Author:     vodozhaba
// Created on: Oct 20, 2017
// Purpose:    One "library" for all bitmaps in the project.

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>

// as long as the length is a size_t, a word index cannot exceed SIZE_T_MAX - 1
#define BITMAP_INVALID_WORD_IX SIZE_MAX
#define BITMAP_SIZE(l, d) (((l) - (d)) / CHAR_BIT)

typedef struct {
    size_t first_free;
} BitmapInternal;

typedef struct {
    void* start;
    size_t len;
    // Use if the bitmap should not describe lower elements
    size_t dead_zone;
    BitmapInternal internal;
} Bitmap;

void InitBitmap(Bitmap* bitmap, bool allocated);
void MarkInBitmap(Bitmap* bitmap, size_t first, size_t len, bool allocated);
size_t AllocateInBitmap(Bitmap* bitmap, size_t len);
void FreeInBitmap(Bitmap* bitmap, size_t first, size_t len);
