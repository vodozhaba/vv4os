// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       arch/x86/mem/phys_mem_mgr_x86.c
// Author:     vodozhaba
// Created on: Sep 30, 2016
// Purpose:    Manages physical memory, allows (de)allocating it on x86.

#include "mem/phys_mem_mgr.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "mem/kernel_mem.h"
#include "util/bitmap.h"

#define BITMAP_LEN (4294967296 / FRAME_SIZE)

static Bitmap bitmap;
static uint8_t bitmap_data[BITMAP_SIZE(BITMAP_LEN, 0)];

void X86PhysMemMgrInit(size_t ram_size) {
    bitmap.start = bitmap_data;
    bitmap.len = ram_size / FRAME_SIZE;
    uint32_t kernel_len = ((uint32_t) KERNEL_STATIC_MEM_END - ((uint32_t) KERNEL_STATIC_MEM_START & 0xFFC00000));
    uint32_t kernel_frames = kernel_len / FRAME_SIZE;
    if(kernel_len % FRAME_SIZE) {
        kernel_frames++;
    }
    bitmap.dead_zone = kernel_frames + 1;
}

void* X86PhysAllocateFrame() {
    size_t k = AllocateInBitmap(&bitmap, 1);
    if(k == BITMAP_INVALID_WORD_IX) {
        return NULL;
    }
    return (void*)(k * FRAME_SIZE);
}

void X86PhysFreeFrame(void* ptr) {
    FreeInBitmap(&bitmap, (uintptr_t) ptr / FRAME_SIZE, 1);
}
