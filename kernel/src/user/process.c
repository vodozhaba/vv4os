// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       user/process.c
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

void X86InitProcess(Process* process);
void X86RestoreProcess(Process* process);
void X86RemoveProcess(Process* process);

static uint32_t last_pid = 0;
static Process* head = NULL;
static Process* current = NULL;

void SchedulerTick() {
    if(!head) {
        printf("No more processes in the system. Halt.\n");
        exit(0);
    }
    if(!current || !current->next) {
        current = head;
    } else {
        current = current->next;
    }
    if(current) {
        X86RestoreProcess(current);
    }
}


void RemoveProcess(uint32_t pid) {
    if(head->pid == pid) {
        head = head->next;
        return;
    }
    Process* process = GetProcess(pid);
    Process* prev;
    for(prev = head; prev->next != NULL && prev->next->pid != pid; prev = prev->next);
    if(prev->next->pid == pid) {
        prev->next = prev->next->next;
    }
    #if defined(__X86__)
    X86RemoveProcess(process);
    #else
    #error "Cannot determine target architecture"
    #endif
}

Process* GetProcess(uint32_t pid) {
    Process* process;
    for(process = head; process != NULL && process->pid != pid; process = process->next);
    return process;
}

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
    #if defined(__X86__)
    X86InitProcess(process);
    #else
    #error "Cannot determine target architecture"
    #endif
    process->next = head;
    head = process;
    return process->pid;
}

uint32_t UserProcessCurrent() {
    return current ? current->pid : 0;
}

FileDescriptor* UserProcessLocalFile(uint32_t pid, uint32_t local_id) {
    Process* process = GetProcess(pid);
    if(!process) {
        return NULL;
    }
    for(FileDescriptor* file = process->local_files; file != NULL; file = file->next) {
        if(file->local_id == local_id) {
            return file;
        }
    }
    return NULL;
}