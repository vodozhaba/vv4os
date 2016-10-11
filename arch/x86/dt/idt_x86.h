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

extern void X86Irq0();
extern void X86Irq1();
extern void X86Irq2();
extern void X86Irq3();
extern void X86Irq4();
extern void X86Irq5();
extern void X86Irq6();
extern void X86Irq7();
extern void X86Irq8();
extern void X86Irq9();
extern void X86Irq10();
extern void X86Irq11();
extern void X86Irq12();
extern void X86Irq13();
extern void X86Irq14();
extern void X86Irq15();

// I've gone mad

extern void X86SoftInt30h();
extern void X86SoftInt31h();
extern void X86SoftInt32h();
extern void X86SoftInt33h();
extern void X86SoftInt34h();
extern void X86SoftInt35h();
extern void X86SoftInt36h();
extern void X86SoftInt37h();
extern void X86SoftInt38h();
extern void X86SoftInt39h();
extern void X86SoftInt3Ah();
extern void X86SoftInt3Bh();
extern void X86SoftInt3Ch();
extern void X86SoftInt3Dh();
extern void X86SoftInt3Eh();
extern void X86SoftInt3Fh();
extern void X86SoftInt40h();
extern void X86SoftInt41h();
extern void X86SoftInt42h();
extern void X86SoftInt43h();
extern void X86SoftInt44h();
extern void X86SoftInt45h();
extern void X86SoftInt46h();
extern void X86SoftInt47h();
extern void X86SoftInt48h();
extern void X86SoftInt49h();
extern void X86SoftInt4Ah();
extern void X86SoftInt4Bh();
extern void X86SoftInt4Ch();
extern void X86SoftInt4Dh();
extern void X86SoftInt4Eh();
extern void X86SoftInt4Fh();
extern void X86SoftInt50h();
extern void X86SoftInt51h();
extern void X86SoftInt52h();
extern void X86SoftInt53h();
extern void X86SoftInt54h();
extern void X86SoftInt55h();
extern void X86SoftInt56h();
extern void X86SoftInt57h();
extern void X86SoftInt58h();
extern void X86SoftInt59h();
extern void X86SoftInt5Ah();
extern void X86SoftInt5Bh();
extern void X86SoftInt5Ch();
extern void X86SoftInt5Dh();
extern void X86SoftInt5Eh();
extern void X86SoftInt5Fh();
extern void X86SoftInt60h();
extern void X86SoftInt61h();
extern void X86SoftInt62h();
extern void X86SoftInt63h();
extern void X86SoftInt64h();
extern void X86SoftInt65h();
extern void X86SoftInt66h();
extern void X86SoftInt67h();
extern void X86SoftInt68h();
extern void X86SoftInt69h();
extern void X86SoftInt6Ah();
extern void X86SoftInt6Bh();
extern void X86SoftInt6Ch();
extern void X86SoftInt6Dh();
extern void X86SoftInt6Eh();
extern void X86SoftInt6Fh();
extern void X86SoftInt70h();
extern void X86SoftInt71h();
extern void X86SoftInt72h();
extern void X86SoftInt73h();
extern void X86SoftInt74h();
extern void X86SoftInt75h();
extern void X86SoftInt76h();
extern void X86SoftInt77h();
extern void X86SoftInt78h();
extern void X86SoftInt79h();
extern void X86SoftInt7Ah();
extern void X86SoftInt7Bh();
extern void X86SoftInt7Ch();
extern void X86SoftInt7Dh();
extern void X86SoftInt7Eh();
extern void X86SoftInt7Fh();
extern void X86SoftInt80h();
extern void X86SoftInt81h();
extern void X86SoftInt82h();
extern void X86SoftInt83h();
extern void X86SoftInt84h();
extern void X86SoftInt85h();
extern void X86SoftInt86h();
extern void X86SoftInt87h();
extern void X86SoftInt88h();
extern void X86SoftInt89h();
extern void X86SoftInt8Ah();
extern void X86SoftInt8Bh();
extern void X86SoftInt8Ch();
extern void X86SoftInt8Dh();
extern void X86SoftInt8Eh();
extern void X86SoftInt8Fh();
extern void X86SoftInt90h();
extern void X86SoftInt91h();
extern void X86SoftInt92h();
extern void X86SoftInt93h();
extern void X86SoftInt94h();
extern void X86SoftInt95h();
extern void X86SoftInt96h();
extern void X86SoftInt97h();
extern void X86SoftInt98h();
extern void X86SoftInt99h();
extern void X86SoftInt9Ah();
extern void X86SoftInt9Bh();
extern void X86SoftInt9Ch();
extern void X86SoftInt9Dh();
extern void X86SoftInt9Eh();
extern void X86SoftInt9Fh();
extern void X86SoftIntA0h();
extern void X86SoftIntA1h();
extern void X86SoftIntA2h();
extern void X86SoftIntA3h();
extern void X86SoftIntA4h();
extern void X86SoftIntA5h();
extern void X86SoftIntA6h();
extern void X86SoftIntA7h();
extern void X86SoftIntA8h();
extern void X86SoftIntA9h();
extern void X86SoftIntAAh();
extern void X86SoftIntABh();
extern void X86SoftIntACh();
extern void X86SoftIntADh();
extern void X86SoftIntAEh();
extern void X86SoftIntAFh();
extern void X86SoftIntB0h();
extern void X86SoftIntB1h();
extern void X86SoftIntB2h();
extern void X86SoftIntB3h();
extern void X86SoftIntB4h();
extern void X86SoftIntB5h();
extern void X86SoftIntB6h();
extern void X86SoftIntB7h();
extern void X86SoftIntB8h();
extern void X86SoftIntB9h();
extern void X86SoftIntBAh();
extern void X86SoftIntBBh();
extern void X86SoftIntBCh();
extern void X86SoftIntBDh();
extern void X86SoftIntBEh();
extern void X86SoftIntBFh();
extern void X86SoftIntC0h();
extern void X86SoftIntC1h();
extern void X86SoftIntC2h();
extern void X86SoftIntC3h();
extern void X86SoftIntC4h();
extern void X86SoftIntC5h();
extern void X86SoftIntC6h();
extern void X86SoftIntC7h();
extern void X86SoftIntC8h();
extern void X86SoftIntC9h();
extern void X86SoftIntCAh();
extern void X86SoftIntCBh();
extern void X86SoftIntCCh();
extern void X86SoftIntCDh();
extern void X86SoftIntCEh();
extern void X86SoftIntCFh();
extern void X86SoftIntD0h();
extern void X86SoftIntD1h();
extern void X86SoftIntD2h();
extern void X86SoftIntD3h();
extern void X86SoftIntD4h();
extern void X86SoftIntD5h();
extern void X86SoftIntD6h();
extern void X86SoftIntD7h();
extern void X86SoftIntD8h();
extern void X86SoftIntD9h();
extern void X86SoftIntDAh();
extern void X86SoftIntDBh();
extern void X86SoftIntDCh();
extern void X86SoftIntDDh();
extern void X86SoftIntDEh();
extern void X86SoftIntDFh();
extern void X86SoftIntE0h();
extern void X86SoftIntE1h();
extern void X86SoftIntE2h();
extern void X86SoftIntE3h();
extern void X86SoftIntE4h();
extern void X86SoftIntE5h();
extern void X86SoftIntE6h();
extern void X86SoftIntE7h();
extern void X86SoftIntE8h();
extern void X86SoftIntE9h();
extern void X86SoftIntEAh();
extern void X86SoftIntEBh();
extern void X86SoftIntECh();
extern void X86SoftIntEDh();
extern void X86SoftIntEEh();
extern void X86SoftIntEFh();
extern void X86SoftIntF0h();
extern void X86SoftIntF1h();
extern void X86SoftIntF2h();
extern void X86SoftIntF3h();
extern void X86SoftIntF4h();
extern void X86SoftIntF5h();
extern void X86SoftIntF6h();
extern void X86SoftIntF7h();
extern void X86SoftIntF8h();
extern void X86SoftIntF9h();
extern void X86SoftIntFAh();
extern void X86SoftIntFBh();
extern void X86SoftIntFCh();
extern void X86SoftIntFDh();
extern void X86SoftIntFEh();
extern void X86SoftIntFFh();

void X86IdtInit();
