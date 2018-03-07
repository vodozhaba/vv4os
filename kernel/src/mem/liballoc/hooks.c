// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       mem/liballoc/hooks.c
// Author:     vodozhaba
// Created on: Feb 08, 2018
// Purpose:    Interface the liballoc library for memory management.

#include "mem/virt_mem_mgr.h"

int liballoc_lock() {
    #if defined(__X86__)
    __asm volatile("cli");
    return 0;
    #else
    return -1;
    #endif
}

int liballoc_unlock() {
    #if defined(__X86__)
    __asm volatile("sti");
    return 0;
    #else
    return -1;
    #endif
}

void* liballoc_alloc(size_t pages) {
    return AllocateContiguousVirtualFrames(pages, true);
}

int liballoc_free(void* base, size_t pages) {
    FreeContiguousVirtualFrames(base, pages);
    return 0;
}