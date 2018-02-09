// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       user/process.h
// Author:     vodozhaba
// Created on: Feb 08, 2018
// Purpose:    Works with userland processes.

#include "user/process.h"
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "mem/phys_mem_mgr.h"
#include "mem/virt_mem_mgr.h"

extern void UserlandJump(void* address, void* address_space);

static uint32_t last_pid = 0;
static Process* head = NULL;

uint32_t UserProcessLoad(FileDescriptor* file) {
    if(last_pid == UINT32_MAX - 1) {
        return 0;
    }
    size_t frames = file->size / FRAME_SIZE;
    if(file->size % FRAME_SIZE) {
        frames++;
    }
    void* buf = AllocateContiguousVirtualFrames(frames, false);
    file->seek = 0;
    file->read_op(file, file->size, buf);
    Process* process = malloc(sizeof(*process));
    process->address_space = CreateAddressSpace(buf, frames);
    process->pid = ++last_pid;
    process->next = head;
    head = process;
    return process->pid;
}