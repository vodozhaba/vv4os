// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       mem/liballoc/hooks.c
// Author:     vodozhaba
// Created on: Feb 08, 2018
// Purpose:    Interface the liballoc library for memory management.

#include "mem/virt_mem_mgr.h"
#include "util/sync.h"

static Mutex liballoc_mutex = { .locked = false };

int liballoc_lock() {
    MutexLock(&liballoc_mutex);
    return 0;
}

int liballoc_unlock() {
    MutexRelease(&liballoc_mutex);
    return 0;
}

void* liballoc_alloc(size_t pages) {
    return AllocateContiguousVirtualFrames(pages, true);
}

int liballoc_free(void* base, size_t pages) {
    FreeContiguousVirtualFrames(base, pages);
    return 0;
}