// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       user/process.h
// Author:     vodozhaba
// Created on: Feb 08, 2018
// Purpose:    Works with userland processes.

#include "user/process.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "mem/phys_mem_mgr.h"
#include "mem/virt_mem_mgr.h"

extern void UserlandJump(void* address, void* address_space);

static uint32_t last_pid = 0;
static Process* head = NULL;

uint32_t UserProcessLoad(FileDescriptor* file, FileDescriptor* stdin, FileDescriptor* stdout, FileDescriptor* stderr) {
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
    FileDescriptor* l_stdin = malloc(sizeof(*l_stdin));
    memcpy(l_stdin, stdin, sizeof(*l_stdin));
    l_stdin->local_id = 0;
    FileDescriptor* l_stdout = malloc(sizeof(*l_stdout));
    memcpy(l_stdout, stdout, sizeof(*l_stdout));
    l_stdout->local_id = 1;
    FileDescriptor* l_stderr = malloc(sizeof(*l_stderr));
    memcpy(l_stderr, stderr, sizeof(*l_stderr));
    l_stderr->local_id = 2;
    process->local_files = l_stdin;
    l_stdin->next = l_stdout;
    l_stdout->next = l_stderr;
    l_stderr->next = NULL;
    // Let's pretend this is a scheduler
    UserlandJump((void*) 0x100000, process->address_space);
    return process->pid;
}

uint32_t UserProcessCurrent() {
    return last_pid;
}

FileDescriptor* UserProcessLocalFile(uint32_t pid, uint32_t local_id) {
    for(Process* process = head; process != NULL; process = process->next) {
        if(process->pid != pid) {
            continue;
        }
        for(FileDescriptor* file = process->local_files; file != NULL; file = file->next) {
            if(file->local_id == local_id) {
                return file;
            }
        }
        return NULL;
    }
    return NULL;
}