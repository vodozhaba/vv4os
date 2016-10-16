/******************************************************************************
 * File:       /vv4os/arch/x86/mem/phys_mem_mgr_x86.c                         *
 * Author:     velikiyv4                                                      *
 * Created on: Sep 30, 2016                                                   *
 * Purpose:    Manages physical memory, allows (de)allocating it on x86.      *
 ******************************************************************************/

#include <stdint.h>
#include <stddef.h>
#include "stdlib/stdlib.h"

static uint32_t bitmap[4096];
static uint32_t total_frames;

void X86PhysMemMgrInit(size_t ram_size) {
    total_frames = ram_size / 4096;
    memset(bitmap, 0xFF, sizeof(bitmap)); // If there's a bug, let it better
                                          // not give free memory than give
                                          // some already allocated
    for(uint32_t frame = 16384; frame < total_frames; frame++) {
        bitmap[frame / 32] &= ~(1 << (frame % 32));
    }
}

void* X86PhysAllocateFrame() {
    for(uint32_t ix = 0; ix < 4096; ix++) {
        if(bitmap[ix] != 0xFFFFFFFF) {
            uint32_t bit;
            for(bit = 0; bitmap[ix] & (1 << bit); bit++);
            bitmap[ix] |= (1 << bit);
            return (void*)(4096 * (32 * ix + bit));
        }
    }
    return NULL;
}

void X86PhysFreeFrame(void* ptr) {
    if(ptr == NULL) {
        return;
    }
    uint32_t frame = (uint32_t) ptr / 4096;
    if(frame >= total_frames) {
        return;
    }
    bitmap[frame / 32] &= ~(1 << (frame % 32));
}
