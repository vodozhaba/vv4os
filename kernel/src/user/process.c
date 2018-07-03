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
#include "core/config.h"
#include "mem/phys_mem_mgr.h"
#include "mem/virt_mem_mgr.h"
#include "util/sync.h"

static Mutex process_table_mutex = { .locked = false };
static uint32_t last_pid = 0;
static Process* head = NULL;
static Process* current = NULL;
static Process* next = NULL;
static bool scheduling;

void StartScheduler() {
    scheduling = true;
    #if defined(__X86__)
    X86StartScheduler();
    #else
    #error "Cannot determine target architecture"
    #endif
}

void StopScheduler() {
    scheduling = false;
    #if defined(__X86__)
    X86StopScheduler();
    #else
    #error "Cannot determine target architecture"
    #endif
}

void SchedulerTick() {
    StopScheduler();
    if(next) {
        current = next;
        next = NULL;
        X86RestoreProcess(current);
    }
    while(true) {
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
            StartScheduler();
            X86RestoreProcess(current);
        }
    }
}

typedef struct {
    bool start_scheduler;
    uint32_t pid;
} RemoveProcessParam;

static void _RemoveProcess(void* param) {
    RemoveProcessParam* s_param = (RemoveProcessParam*) param;
    Process* process = NULL;
    // Keep a reference to the pointer to the next process so that we don't have to keep track of two processes
    MutexLock(&process_table_mutex);
    for(Process** next = &head; next; next = &(*next)->next) {
        if((*next)->pid == s_param->pid) {
            process = *next;
            *next = (*next)->next;
            break;
        }
    }
    if(process) {
        #if defined(__X86__)
        X86DeleteAddressSpace(process->address_space);
        #else
        #error "Cannot determine target architecture"
        #endif
        free(process->kernel_stack - KERNEL_SYSCALL_STACK);
        free(process->last_state);
        for(FileDescriptor* file = process->local_files; file; ) {
            FileDescriptor* next = file->next;
            free(file);
            file = next;
        }
    }
    MutexRelease(&process_table_mutex);
    if(s_param->start_scheduler) {
        StartScheduler();
        while(1);
    }
}

void RemoveProcess(uint32_t pid) {
    RemoveProcessParam* param = malloc(sizeof(*param));
    param->start_scheduler = scheduling;
    param->pid = pid;
    StopScheduler();
    X86RestoreKernel(_RemoveProcess, param);
}

Process* GetProcess(uint32_t pid) {
    Process* process;
    MutexLock(&process_table_mutex);
    for(process = head; process != NULL && process->pid != pid; process = process->next);
    MutexRelease(&process_table_mutex);
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
    process->kernel_stack = malloc(KERNEL_SYSCALL_STACK) + KERNEL_SYSCALL_STACK;
    process->address_space = CreateAddressSpace(buf, frames);
    process->ppid = 0;
    #if defined(__X86__)
    X86GenInitialProcessState(process);
    #else
    #error "Cannot determine target architecture"
    #endif
    MutexLock(&process_table_mutex);
    bool start_scheduler = scheduling;
    StopScheduler();
    process->pid = ++last_pid;
    process->next = head;
    head = process;
    MutexRelease(&process_table_mutex);
    if(start_scheduler) {
        StartScheduler();
    }
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

void* GenReturnProcessState(void* old, size_t ret) {
    #if defined(__X86__)
    return X86GenReturnProcessState(old, ret);
    #else
    #error "Cannot determine target architecture"
    #endif
}

uint32_t CopyProcess(Process* old, void* new_state) {
    Process* new = malloc(sizeof(*new));
    new->address_space = old->address_space;
    new->last_state = new_state;
    new->kernel_stack = malloc(KERNEL_SYSCALL_STACK) + KERNEL_SYSCALL_STACK;
    new->local_files = old->local_files;
    new->ppid = old->pid;
    MutexLock(&process_table_mutex);
    bool start_scheduler = scheduling;
    StopScheduler();
    new->pid = ++last_pid;
    new->next = head;
    head = new;
    MutexRelease(&process_table_mutex);
    if(start_scheduler) {
        StartScheduler();
    }
    return new->pid;
}