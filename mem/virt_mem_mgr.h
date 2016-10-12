/******************************************************************************
 * File:       /vv4os/mem/virt_mem_mgr.h                                      *
 * Author:     velikiyv4                                                      *
 * Created on: Oct 12, 2016                                                   *
 * Purpose:    Aliases virtual memory managers for different architectures.   *
 ******************************************************************************/

#pragma once

void* X86CreateKernelAndProcWindow(void* proc_base, uint32_t frames,
        uint32_t proc_id);
void X86NewVirtMemTable(uint32_t proc_id);
int X86DestroyVirtMemTable(uint32_t proc_id);
void X86VirtMemMgrInit();

#if !defined(__X86__)
#error "Cannot determine target architecture"
#endif

static inline void* CreateKernelAndProcWindow(void* proc_base, uint32_t frames,
        uint32_t proc_id) {
#if defined(__X86__)
    return X86CreateKernelAndProcWindow(proc_base, frames, proc_id);
#endif
}

static inline void NewVirtMemTable(uint32_t proc_id) {
#if defined(__X86__)
    X86NewVirtMemTable(proc_id);
#endif
}

static inline int DestroyVirtMemTable(uint32_t proc_id) {
#if defined(__X86__)
    return X86DestroyVirtMemTable(proc_id);
#endif
}

static inline void VirtMemMgrInit() {
#if defined(__X86__)
    X86VirtMemMgrInit();
#endif
}
