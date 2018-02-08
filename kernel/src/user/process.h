// File:       user/process.h
// Author:     vodozhaba
// Created on: Feb 08, 2018
// Purpose:    Works with userland processes.

#include <stdint.h>
#include "io/disk/file.h"

typedef struct Process {
    uint32_t pid;
    void* address_space;
    struct Process* next;
} Process;

uint32_t UserProcessLoad(FileDescriptor* file);