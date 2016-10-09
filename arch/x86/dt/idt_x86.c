/******************************************************************************
 * File:       /vv4os/arch/x86/dt/idt_x86.c                                   *
 * Author:     velikiyv4                                                      *
 * Created on: Oct 9, 2016                                                    *
 * Purpose:    Allows working with x86 Interrupt Descriptor Table.            *
 ******************************************************************************/

#include <stdbool.h>
#include "idt_x86.h"
#include "stdlib/stdio.h"
#include "stdlib/stdlib.h"

extern void X86SetIdtr(Idtr* idtr);

IdtEntry idt[256];
Idtr idtr;

static void UpdateIdt(IdtEntry* idt) {
    idtr.base = (uint32_t) idt;
    idtr.size = 256;
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

void X86IsrHandler(InterruptedCpuState state) {

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
    RegisterInterrupt(30, X86Isr20, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
    RegisterInterrupt(31, X86Isr21, IDT_ENTRY_TYPE_INTERRUPT_32_RING0);
}
