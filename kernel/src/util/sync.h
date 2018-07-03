// File:       util/sync.h
// Author:     vodozhaba
// Created on: Jul 01, 2016
// Purpose:    Thread synchronization primitives.

#include <stdbool.h>

typedef struct {
    bool locked;
} Mutex;

bool MutexTryLock(Mutex* mutex);
void MutexLock(Mutex* mutex);
void MutexRelease(Mutex* mutex);