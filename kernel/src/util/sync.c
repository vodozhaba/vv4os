// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       util/sync.c
// Author:     vodozhaba
// Created on: Jul 01, 2016
// Purpose:    Thread synchronization primitives.

#include "util/sync.h"

#if defined(__X86__)
#define ATOMIC_BEGIN() __asm volatile("pushf\n\tcli");
#define ATOMIC_END() __asm volatile("popf")
#else
#error "Cannot determine target architecture"
#endif

bool MutexTryLock(Mutex* mutex) {
    ATOMIC_BEGIN();
    if(mutex->locked) {
        ATOMIC_END();
        return false;
    }
    mutex->locked = true;
    ATOMIC_END();
    return true;
}

void MutexLock(Mutex* mutex) {
    while(true) {
        ATOMIC_BEGIN();
        if(mutex->locked) {
            ATOMIC_END();
            continue;
        }
        mutex->locked = true;
        ATOMIC_END();
        return;
    }
}

void MutexRelease(Mutex* mutex) {
    mutex->locked = false;
}