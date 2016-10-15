/******************************************************************************
 * File:       /vv4os/mem/virt_mem_mgr.h                                      *
 * Author:     velikiyv4                                                      *
 * Created on: Oct 12, 2016                                                   *
 * Purpose:    Aliases virtual memory managers for different architectures.   *
 ******************************************************************************/

#pragma once

void X86VirtMemMgrInit();

#if !defined(__X86__)
#error "Cannot determine target architecture"
#endif

static inline void VirtMemMgrInit() {
#if defined(__X86__)
    X86VirtMemMgrInit();
#endif
}