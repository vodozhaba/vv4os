/******************************************************************************
 * File:       /vv4os/arch/x86/dt/idt_x86.h                                   *
 * Author:     velikiyv4                                                      *
 * Created on: Oct 9, 2016                                                    *
 * Purpose:    Allows working with x86 Interrupt Descriptor Table.            *
 ******************************************************************************/

#pragma once

#include <stdint.h>
#include "arch/x86/dt/gdt_x86.h"

typedef struct {
    uint16_t size;
    uint32_t base;
} __attribute__((packed)) Idtr;

typedef struct {
    uint16_t offset_low;
    GdtSelector segment;
    uint8_t zero;
    uint8_t type;
    uint16_t offset_high;
} __attribute__((packed)) IdtEntry;

typedef struct {
    uint32_t ds;
    uint32_t ed;
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
} __attribute__((packed)) InterruptedCpuState;

#define IDT_ENTRY_TYPE_INTERRUPT_32_RING0 0x8E
#define IDT_ENTRY_TYPE_INTERRUPT_16_RING0 0x86
#define IDT_ENTRY_TYPE_TRAP_32_RING0      0x8F
#define IDT_ENTRY_TYPE_TRAP_16_RING0      0x87
#define IDT_ENTRY_TYPE_TASK_32_RING0      0x85
#define IDT_ENTRY_TYPE_INTERRUPT_32_RING1 0xAE
#define IDT_ENTRY_TYPE_INTERRUPT_16_RING1 0xA6
#define IDT_ENTRY_TYPE_TRAP_32_RING1      0xAF
#define IDT_ENTRY_TYPE_TRAP_16_RING1      0xA7
#define IDT_ENTRY_TYPE_TASK_32_RING1      0xA5
#define IDT_ENTRY_TYPE_INTERRUPT_32_RING2 0xCE
#define IDT_ENTRY_TYPE_INTERRUPT_16_RING2 0xC6
#define IDT_ENTRY_TYPE_TRAP_32_RING2      0xCF
#define IDT_ENTRY_TYPE_TRAP_16_RING2      0xC7
#define IDT_ENTRY_TYPE_TASK_32_RING2      0xC5
#define IDT_ENTRY_TYPE_INTERRUPT_32_RING3 0xEE
#define IDT_ENTRY_TYPE_INTERRUPT_16_RING3 0xE6
#define IDT_ENTRY_TYPE_TRAP_32_RING3      0xEF
#define IDT_ENTRY_TYPE_TRAP_16_RING3      0xE7
#define IDT_ENTRY_TYPE_TASK_32_RING3      0xE5

#define IRQ(x) ((x) + 0x20)

extern void X86Isr0();
extern void X86Isr1();
extern void X86Isr2();
extern void X86Isr3();
extern void X86Isr4();
extern void X86Isr5();
extern void X86Isr6();
extern void X86Isr7();
extern void X86Isr8();
extern void X86Isr9();
extern void X86Isr10();
extern void X86Isr11();
extern void X86Isr12();
extern void X86Isr13();
extern void X86Isr14();
extern void X86Isr15();
extern void X86Isr16();
extern void X86Isr17();
extern void X86Isr18();
extern void X86Isr19();
extern void X86Isr20();
extern void X86Isr21();
extern void X86Isr22();
extern void X86Isr23();
extern void X86Isr24();
extern void X86Isr25();
extern void X86Isr26();
extern void X86Isr27();
extern void X86Isr28();
extern void X86Isr29();
extern void X86Isr30();
extern void X86Isr31();

void X86IdtInit();
