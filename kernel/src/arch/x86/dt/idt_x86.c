// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       arch/x86/dt/idt_x86.c
// Author:     vodozhaba
// Created on: Oct 9, 2016
// Purpose:    Allows working with x86 Interrupt Descriptor Table.

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "core/config.h"
#include "idt_x86.h"
#include "io/ports.h"
#include "user/process.h"
#include "user/syscall.h"
#include "mem/virt_mem_mgr.h"

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

extern void X86Syscall();

extern void X86SetIdtr(Idtr* idtr);

static InterruptHandler isr_handlers[32];
static InterruptHandler irq_handlers[16];
static InterruptHandler soft_int_handlers[208];

static IdtEntry idt[256];
static Idtr idtr;

static void UpdateIdt(IdtEntry* idt) {
    idtr.base = (uint32_t) idt;
    idtr.size = sizeof(IdtEntry) * 256;
    X86SetIdtr(&idtr);
}

static void RegisterInterrupt(uint8_t n, void (*handler)(), uint8_t type) {
    __asm volatile("pushf");
    __asm volatile("cli");
    GdtSelector segment = { .rpl = 0, .ldt = false, .index = 1 };
    idt[n].segment = segment;
    idt[n].offset_low = (uint32_t) handler;
    idt[n].offset_high = (uint16_t)((uint32_t) handler >> 16);
    idt[n].type = type;
    idt[n].zero = 0;
    __asm volatile("popf");
}

void X86IsrHandler(__attribute__((unused)) X86CpuState state) {
    if(isr_handlers[state.interrupt]) {
        isr_handlers[state.interrupt](&state);
    }
}

void X86SoftIntHandler(__attribute__((unused)) X86CpuState state) {
    uint32_t interrupt = state.interrupt - 0x30;
    if(soft_int_handlers[interrupt]) {
        soft_int_handlers[interrupt](&state);
    }
}

void X86IrqHandler(X86CpuState state) {
    uint32_t interrupt = state.interrupt - IRQ(0);
    if(irq_handlers[interrupt]) {
        irq_handlers[interrupt](&state);
    }
    if(state.interrupt > IRQ(7)) {
        PortWrite8(0xA0, 0x20);
        return;
    }
    PortWrite8(0x20, 0x20);
}

void X86SyscallHandler(X86CpuState state) {
    state.eax = Syscall(state.eax, state.ebx, state.ecx, state.edx);
}

void X86SchedulerTick(X86CpuState* state) {
    X86SwitchAddressSpace(NULL);
    Process* process = GetProcess(UserProcessCurrent());
    if(process != NULL)
        memcpy(process->last_state, state, sizeof(*state));
    X86RestoreKernel(SchedulerTick, NULL);
}

void X86IdtInit() {
    memset(idt, 0, sizeof(idt));
    UpdateIdt(idt);
    RegisterInterrupt(0, X86Isr0, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(1, X86Isr1, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(2, X86Isr2, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(3, X86Isr3, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(4, X86Isr4, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(5, X86Isr5, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(6, X86Isr6, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(7, X86Isr7, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(8, X86Isr8, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(9, X86Isr9, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(10, X86Isr10, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(11, X86Isr11, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(31, X86Isr31, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(12, X86Isr12, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(13, X86Isr13, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(14, X86Isr14, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(15, X86Isr15, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(16, X86Isr16, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(17, X86Isr17, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(18, X86Isr18, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(19, X86Isr19, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(20, X86Isr20, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(21, X86Isr21, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(22, X86Isr22, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(23, X86Isr23, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(24, X86Isr24, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(25, X86Isr25, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(26, X86Isr26, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(27, X86Isr27, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(28, X86Isr28, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(29, X86Isr29, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(30, X86Isr30, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);

    RegisterInterrupt(IRQ(0), X86Irq0, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(IRQ(1), X86Irq1, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(IRQ(2), X86Irq2, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(IRQ(3), X86Irq3, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(IRQ(4), X86Irq4, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(IRQ(5), X86Irq5, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(IRQ(6), X86Irq6, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(IRQ(7), X86Irq7, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(IRQ(8), X86Irq8, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(IRQ(9), X86Irq9, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(IRQ(10), X86Irq10, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(IRQ(11), X86Irq11, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(IRQ(12), X86Irq12, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(IRQ(13), X86Irq13, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(IRQ(14), X86Irq14, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(IRQ(15), X86Irq15, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);

    RegisterInterrupt(0x30, X86SoftInt30h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x31, X86SoftInt31h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x32, X86SoftInt32h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x33, X86SoftInt33h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x34, X86SoftInt34h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x35, X86SoftInt35h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x36, X86SoftInt36h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x37, X86SoftInt37h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x38, X86SoftInt38h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x39, X86SoftInt39h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x3A, X86SoftInt3Ah, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x3B, X86SoftInt3Bh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x3C, X86SoftInt3Ch, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x3D, X86SoftInt3Dh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x3E, X86SoftInt3Eh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x3F, X86SoftInt3Fh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x40, X86SoftInt40h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x41, X86SoftInt41h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x42, X86SoftInt42h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x43, X86SoftInt43h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x44, X86SoftInt44h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x45, X86SoftInt45h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x46, X86SoftInt46h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x47, X86SoftInt47h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x48, X86SoftInt48h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x49, X86SoftInt49h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x4A, X86SoftInt4Ah, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x4B, X86SoftInt4Bh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x4C, X86SoftInt4Ch, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x4D, X86SoftInt4Dh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x4E, X86SoftInt4Eh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x4F, X86SoftInt4Fh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x50, X86SoftInt50h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x51, X86SoftInt51h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x52, X86SoftInt52h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x53, X86SoftInt53h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x54, X86SoftInt54h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x55, X86SoftInt55h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x56, X86SoftInt56h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x57, X86SoftInt57h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x58, X86SoftInt58h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x59, X86SoftInt59h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x5A, X86SoftInt5Ah, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x5B, X86SoftInt5Bh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x5C, X86SoftInt5Ch, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x5D, X86SoftInt5Dh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x5E, X86SoftInt5Eh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x5F, X86SoftInt5Fh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x60, X86SoftInt60h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x61, X86SoftInt61h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x62, X86SoftInt62h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x63, X86SoftInt63h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x64, X86SoftInt64h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x65, X86SoftInt65h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x66, X86SoftInt66h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x67, X86SoftInt67h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x68, X86SoftInt68h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x69, X86SoftInt69h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x6A, X86SoftInt6Ah, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x6B, X86SoftInt6Bh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x6C, X86SoftInt6Ch, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x6D, X86SoftInt6Dh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x6E, X86SoftInt6Eh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x6F, X86SoftInt6Fh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x70, X86SoftInt70h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x71, X86SoftInt71h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x72, X86SoftInt72h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x73, X86SoftInt73h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x74, X86SoftInt74h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x75, X86SoftInt75h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x76, X86SoftInt76h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x77, X86SoftInt77h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x78, X86SoftInt78h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x79, X86SoftInt79h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x7A, X86SoftInt7Ah, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x7B, X86SoftInt7Bh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x7C, X86SoftInt7Ch, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x7D, X86SoftInt7Dh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x7E, X86SoftInt7Eh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x7F, X86SoftInt7Fh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x80, X86SoftInt80h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x81, X86SoftInt81h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x82, X86SoftInt82h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x83, X86SoftInt83h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x84, X86SoftInt84h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x85, X86SoftInt85h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x86, X86SoftInt86h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x87, X86SoftInt87h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x88, X86SoftInt88h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x89, X86SoftInt89h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x8A, X86SoftInt8Ah, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x8B, X86SoftInt8Bh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x8C, X86SoftInt8Ch, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x8D, X86SoftInt8Dh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x8E, X86SoftInt8Eh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x8F, X86SoftInt8Fh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x90, X86SoftInt90h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x91, X86SoftInt91h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x92, X86SoftInt92h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x93, X86SoftInt93h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x94, X86SoftInt94h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x95, X86SoftInt95h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x96, X86SoftInt96h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x97, X86SoftInt97h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x98, X86SoftInt98h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x99, X86SoftInt99h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x9A, X86SoftInt9Ah, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x9B, X86SoftInt9Bh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x9C, X86SoftInt9Ch, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x9D, X86SoftInt9Dh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x9E, X86SoftInt9Eh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0x9F, X86SoftInt9Fh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xA0, X86SoftIntA0h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xA1, X86SoftIntA1h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xA2, X86SoftIntA2h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xA3, X86SoftIntA3h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xA4, X86SoftIntA4h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xA5, X86SoftIntA5h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xA6, X86SoftIntA6h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xA7, X86SoftIntA7h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xA8, X86SoftIntA8h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xA9, X86SoftIntA9h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xAA, X86SoftIntAAh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xAB, X86SoftIntABh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xAC, X86SoftIntACh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xAD, X86SoftIntADh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xAE, X86SoftIntAEh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xAF, X86SoftIntAFh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xB0, X86SoftIntB0h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xB1, X86SoftIntB1h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xB2, X86SoftIntB2h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xB3, X86SoftIntB3h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xB4, X86SoftIntB4h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xB5, X86SoftIntB5h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xB6, X86SoftIntB6h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xB7, X86SoftIntB7h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xB8, X86SoftIntB8h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xB9, X86SoftIntB9h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xBA, X86SoftIntBAh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xBB, X86SoftIntBBh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xBC, X86SoftIntBCh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xBD, X86SoftIntBDh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xBE, X86SoftIntBEh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xBF, X86SoftIntBFh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xC0, X86SoftIntC0h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xC1, X86SoftIntC1h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xC2, X86SoftIntC2h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xC3, X86SoftIntC3h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xC4, X86SoftIntC4h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xC5, X86SoftIntC5h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xC6, X86SoftIntC6h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xC7, X86SoftIntC7h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xC8, X86SoftIntC8h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xC9, X86SoftIntC9h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xCA, X86SoftIntCAh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xCB, X86SoftIntCBh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xCC, X86SoftIntCCh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xCD, X86SoftIntCDh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xCE, X86SoftIntCEh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xCF, X86SoftIntCFh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xD0, X86SoftIntD0h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xD1, X86SoftIntD1h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xD2, X86SoftIntD2h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xD3, X86SoftIntD3h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xD4, X86SoftIntD4h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xD5, X86SoftIntD5h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xD6, X86SoftIntD6h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xD7, X86SoftIntD7h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xD8, X86SoftIntD8h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xD9, X86SoftIntD9h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xDA, X86SoftIntDAh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xDB, X86SoftIntDBh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xDC, X86SoftIntDCh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xDD, X86SoftIntDDh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xDE, X86SoftIntDEh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xDF, X86SoftIntDFh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xE0, X86SoftIntE0h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xE1, X86SoftIntE1h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xE2, X86SoftIntE2h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xE3, X86SoftIntE3h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xE4, X86SoftIntE4h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xE5, X86SoftIntE5h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xE6, X86SoftIntE6h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xE7, X86SoftIntE7h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xE8, X86SoftIntE8h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xE9, X86SoftIntE9h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xEA, X86SoftIntEAh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xEB, X86SoftIntEBh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xEC, X86SoftIntECh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xED, X86SoftIntEDh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xEE, X86SoftIntEEh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xEF, X86SoftIntEFh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xF0, X86SoftIntF0h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xF1, X86SoftIntF1h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xF2, X86SoftIntF2h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xF3, X86SoftIntF3h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xF4, X86SoftIntF4h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xF5, X86SoftIntF5h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xF6, X86SoftIntF6h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xF7, X86SoftIntF7h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xF8, X86SoftIntF8h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xF9, X86SoftIntF9h, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xFA, X86SoftIntFAh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xFB, X86SoftIntFBh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xFC, X86SoftIntFCh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xFD, X86SoftIntFDh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xFE, X86SoftIntFEh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(0xFF, X86SoftIntFFh, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);

    RegisterInterrupt(USER_SYSCALL_INTERRUPT, X86Syscall, IDT_ENTRY_TYPE_INTERRUPT_32_RING3);

    X86RegisterIrqHandler(IRQ(0), X86SchedulerTick);

    // Remap master PIC
    PortWrite8(0x20, 0x11);
    PortWrite8(0x21, IRQ(0));
    PortWrite8(0x21, 0x04);
    PortWrite8(0x21, 0x01);
    PortWrite8(0x21, 0x00);
    // Remap slave PIC
    PortWrite8(0xA0, 0x11);
    PortWrite8(0xA1, IRQ(8));
    PortWrite8(0xA1, 0x02);
    PortWrite8(0xA1, 0x01);
    PortWrite8(0xA1, 0x00);

    uint16_t div = 1.193182 * USER_PROCESS_TIME_SLICE_US;

    PortWrite8(0x40, div);
    PortWrite8(0x40, div >> 8);

    X86StopScheduler();
    __asm volatile("sti");
}

void X86RegisterIsrHandler(uint8_t n, InterruptHandler handler) {
    isr_handlers[n] = handler;
}

void X86RegisterSoftIntHandler(uint8_t n, InterruptHandler handler) {
    soft_int_handlers[n - 0x30] = handler;
}

void X86RegisterIrqHandler(uint8_t n, InterruptHandler handler) {
    irq_handlers[n - IRQ(0)] = handler;
}
