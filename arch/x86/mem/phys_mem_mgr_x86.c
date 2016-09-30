/******************************************************************************
 * File:       /vv4os/arch/x86/mem/phys_mem_mgr_x86.c                         *
 * Author:     velikiyv4                                                      *
 * Created on: Sep 30, 2016                                                   *
 * Purpose:    Manages physical memory, allows (de)allocating it on x86.      *
 ******************************************************************************/

#include <stdint.h>
#include <stddef.h>
#include "stdlib/stdlib.h"

static uint32_t total_ram;
static uint32_t bitmap[4096];

void X86PhysMemMgrInit(size_t ram_size) {
    total_ram = ram_size;
    memset(bitmap, 0, sizeof(bitmap));
    // TODO allocate all frames outside of memory
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
    uint32_t frame = (uint32_t) ptr / 4096;
    bitmap[frame / 8] &= ~(1 << (frame % 8));
}
