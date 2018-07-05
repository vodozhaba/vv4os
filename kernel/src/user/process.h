// File:       user/process.h
// Author:     vodozhaba
// Created on: Feb 08, 2018
// Purpose:    Works with userland processes.

#include <stdarg.h>
#include <stdint.h>
#include "io/disk/file.h"

typedef struct Process {
    void* address_space;
    void* last_state;
    void* kernel_stack;
    FileDescriptor* local_files;
    struct Process* next;
    uint32_t pid;
    uint32_t ppid;
} Process;

void X86GenInitialProcessState(Process* process);
void* X86GenReturnProcessState(void* old, size_t ret);
void X86RestoreProcess(Process* process);
void X86RestoreKernel(void (*entry)(void*), void* param);
Process* X86CopyProcess(Process* old, void* new_state);
void X86DeleteAddressSpace(void* address_space);
void X86StartScheduler();
void X86StopScheduler();

void StartScheduler();
void StopScheduler();
void SchedulerTick();
void* GenReturnProcessState(void* old, size_t ret);
void RemoveProcess(uint32_t pid);
uint32_t CopyProcess(Process* old, void* new_state);
Process* GetProcess(uint32_t pid);
uint32_t UserProcessLoad(FileDescriptor* file, FileDescriptor* stdin, FileDescriptor* stdout, FileDescriptor* stderr);
Process* UserProcessCurrent();
FileDescriptor* UserProcessLocalFile(Process* process, uint32_t local_id);