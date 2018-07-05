// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       io/ports.h
// Author:     vodozhaba
// Created on: Nov 18, 2017
// Purpose:    Provides functions for port I/O.

#include "ports.h"
#include <limits.h>
#include <stdlib.h>
#include "util/bitmap.h"

#define BITMAP_LEN (UINT16_MAX + 1)
#define BITMAP_DZ 0x0400

static Bitmap bitmap;
static void* bitmap_data;

void IoPortMgrInit() {
    bitmap_data = malloc(BITMAP_SIZE(BITMAP_LEN, BITMAP_DZ));
    bitmap.start = bitmap_data;
    bitmap.len = UINT16_MAX + 1;
    bitmap.dead_zone = BITMAP_DZ;
    InitBitmap(&bitmap, false);
}

uint16_t AllocPorts(uint16_t len) {
    size_t ret = AllocateInBitmap(&bitmap, len);
    if(ret == BITMAP_INVALID_WORD_IX) {
        return PORT_ILLEGAL_VALUE;
    }
    return ret;
}

void FreePorts(uint16_t start, uint16_t len) {
    MarkInBitmap(&bitmap, start, len, false);
}
