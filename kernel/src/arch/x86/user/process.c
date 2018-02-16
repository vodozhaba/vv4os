// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       arch/x86/user/process.h
// Author:     vodozhaba
// Created on: Feb 08, 2018
// Purpose:    Works with userland processes (x86).

#include "user/process.h"
#include <stdlib.h>
#include "arch/x86/dt/gdt_x86.h"
#include "arch/x86/dt/idt_x86.h"
#include "core/config.h"
#include "io/ports.h"
#include "mem/kernel_mem.h"
#include "mem/virt_mem_mgr.h"

extern void X86UserlandJump(void* address_space, X86CpuState cpu_state);

void X86StopScheduler() {
    PortWrite8(0x21, PortRead8(0x21) | 0x01);
}

void X86StartScheduler() {
    PortWrite8(0x21, PortRead8(0x21) & 0xFE);
}

void X86InitProcess(Process* process) {
    X86CpuState* state = calloc(1, sizeof(X86CpuState));
    state->cs = 0x1B;
    state->ds = 0x23;
    state->eflags = 0x0202;
    state->eip = (uint32_t) USER_PROCESS_BASE;
    state->esp = (uint32_t) X86CreateStack(process->address_space, (void*)((uint32_t) KERNEL_STATIC_MEM_START & 0xFFFFF000), USER_PROCESS_STACK);
    state->ss = 0x23;
    state->user_esp = state->esp;
    process->last_state = state;
    process->kernel_stack = malloc(KERNEL_SYSCALL_STACK) + KERNEL_SYSCALL_STACK;
}

void X86RestoreProcess(Process* process) {
    X86CpuState* cpu_state = process->last_state;
    X86SetKernelStack(process->kernel_stack);
    X86UserlandJump(process->address_space, *cpu_state);
}