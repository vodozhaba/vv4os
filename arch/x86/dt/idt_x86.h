/******************************************************************************
 * File:       /vv4os/arch/x86/dt/gdt.h                                       *
 * Author:     velikiyv4                                                      *
 * Created on: Oct 1, 2016                                                    *
 * Purpose:    Allows working with x86 Interrupt Descriptor Table.            *
 ******************************************************************************/

#pragma once

#include "gdt_x86.h"

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) Idtr;

typedef struct {
    uint16_t offset_low;
    GdtSelector selector;
    uint8_t reserved;
    uint8_t type;
    uint16_t offset_high;
} __attribute__((packed)) IdtEntry;

#define IDT_ENTRY_TYPE_386_TASK_RING0      0x85
#define IDT_ENTRY_TYPE_286_INTERRUPT_RING0 0x86
#define IDT_ENTRY_TYPE_286_TRAP_RING0      0x87
#define IDT_ENTRY_TYPE_386_INTERRUPT_RING0 0x8E
#define IDT_ENTRY_TYPE_386_TRAP_RING0      0x8F
#define IDT_ENTRY_TYPE_386_TASK_RING1      0xA5
#define IDT_ENTRY_TYPE_286_INTERRUPT_RING1 0xA6
#define IDT_ENTRY_TYPE_286_TRAP_RING1      0xA7
#define IDT_ENTRY_TYPE_386_INTERRUPT_RING1 0xAE
#define IDT_ENTRY_TYPE_386_TRAP_RING1      0xAF
#define IDT_ENTRY_TYPE_386_TASK_RING2      0xC5
#define IDT_ENTRY_TYPE_286_INTERRUPT_RING2 0xC6
#define IDT_ENTRY_TYPE_286_TRAP_RING2      0xC7
#define IDT_ENTRY_TYPE_386_INTERRUPT_RING2 0xCE
#define IDT_ENTRY_TYPE_386_TRAP_RING2      0xCF
#define IDT_ENTRY_TYPE_386_TASK_RING3      0xE5
#define IDT_ENTRY_TYPE_286_INTERRUPT_RING3 0xE6
#define IDT_ENTRY_TYPE_286_TRAP_RING3      0xE7
#define IDT_ENTRY_TYPE_386_INTERRUPT_RING3 0xEE
#define IDT_ENTRY_TYPE_386_TRAP_RING3      0xEF

#define IRQ_BASE 0x20

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

typedef void (*IntHandler)();

void X86RegisterInterrupt(uint8_t n, IntHandler handler, uint8_t type);
void X86IdtInit();

#define DECLARE_INT_HANDLER(name) \
        void name(); \
        __asm (#name":\n" \
               "pusha\n" \
               "call "#name"_InterruptHandler\n" \
               "movb $0x20, %al\n" \
               "outb %al, $0x20\n" \
               "outb %al, $0xA0\n" \
               "popa\n" \
               "iret"); \
        void name##_InterruptHandler()
