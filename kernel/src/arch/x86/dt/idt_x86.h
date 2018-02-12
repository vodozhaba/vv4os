// File:       arch/x86/dt/idt_x86.h
// Author:     vodozhaba
// Created on: Oct 9, 2016
// Purpose:    Allows working with x86 Interrupt Descriptor Table.

#pragma once

#include <stdint.h>
#include "arch/x86/dt/gdt_x86.h"

typedef struct {
    uint32_t ds;
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t interrupt;
    uint32_t error_code;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t user_esp;
    uint32_t ss;
} __attribute__((packed)) X86CpuState;

typedef void (*InterruptHandler)(X86CpuState*);

#define IRQ(x) ((x) + 0x20)

void X86IdtInit();
void X86RegisterIsrHandler(uint8_t n, InterruptHandler handler);
void X86RegisterSoftIntHandler(uint8_t n, InterruptHandler handler);
void X86RegisterIrqHandler(uint8_t n, InterruptHandler handler);
