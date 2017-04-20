/******************************************************************************
 * File:       /vv4os/mem/virt_mem_mgr.h                                      *
 * Author:     velikiyv4                                                      *
 * Created on: Oct 12, 2016                                                   *
 * Purpose:    Aliases virtual memory managers for different architectures.   *
 ******************************************************************************/

#pragma once

#include <stddef.h>
#include "phys_mem_mgr.h"

void X86VirtMemMgrInit();
void* X86AllocateContiguousVirtualFrames(uint32_t frames);
void X86FreeContiguousVirtualFrames(void* base, uint32_t frames);

#if !defined(__X86__)
#error "Cannot determine target architecture"
#endif

static inline void VirtMemMgrInit() {
#if defined(__X86__)
    X86VirtMemMgrInit();
#endif
}

static inline void* AllocateContiguousVirtualFrames(size_t frames) {
#if defined(__X86__)
    return X86AllocateContiguousVirtualFrames(frames);
#endif
}

static inline void FreeContiguousVirtualFrames(void* base, size_t frames) {
#if defined(__X86__)
    return X86FreeContiguousVirtualFrames(base, frames);
#endif
}
