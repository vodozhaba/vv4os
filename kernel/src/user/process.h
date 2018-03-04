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
} Process;

void X86GenInitialProcessState(Process* process);
void X86RestoreProcess(Process* process);
void X86RestoreKernel(void (*entry)(void*), void* param);  
void X86DeleteAddressSpace(void* address_space);
void X86StartScheduler();
void X86StopScheduler();

void StartScheduler();
void StopScheduler();
void SchedulerTick();
void RemoveProcess(uint32_t pid);
Process* GetProcess(uint32_t pid);
uint32_t UserProcessLoad(FileDescriptor* file, FileDescriptor* stdin, FileDescriptor* stdout, FileDescriptor* stderr);
uint32_t UserProcessCurrent();
FileDescriptor* UserProcessLocalFile(uint32_t pid, uint32_t local_id);