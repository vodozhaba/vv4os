// File:       user/process.h
// Author:     vodozhaba
// Created on: Feb 08, 2018
// Purpose:    Works with userland processes.

#include <stdarg.h>
#include <stdint.h>
#include "io/disk/file.h"

typedef struct Process {
    uint32_t pid;
    void* address_space;
    FileDescriptor* local_files;
    struct Process* next;
} Process;

uint32_t UserProcessLoad(FileDescriptor* file, FileDescriptor* stdin, FileDescriptor* stdout, FileDescriptor* stderr);
uint32_t UserProcessCurrent();
FileDescriptor* UserProcessLocalFile(uint32_t pid, uint32_t local_id);