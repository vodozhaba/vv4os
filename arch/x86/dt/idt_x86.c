/******************************************************************************
 * File:       /vv4os/arch/x86/dt/gdt.h                                       *
 * Author:     velikiyv4                                                      *
 * Created on: Oct 2, 2016                                                    *
 * Purpose:    Allows working with x86 Interrupt Descriptor Table.            *
 ******************************************************************************/

#include "idt_x86.h"
#include "io/ports.h"
#include <stdbool.h>
#include "stdlib/stdlib.h"

extern void SetIdtr(Idtr* idtr);

IdtEntry idt[256];
Idtr idtr;

void X86RegisterInterrupt(uint8_t n, IntHandler handler, uint8_t type) {
    __asm volatile("pushf");
    __asm volatile("cli");
    IdtEntry entry;
    uint32_t offset = (uint32_t) handler;
    entry.offset_low = offset;
    entry.offset_high = offset >> 16;
    entry.reserved = 0;
    entry.type = type;
    entry.selector.index = 1;
    entry.selector.ldt = false;
    entry.selector.rpl = 0;
    idt[n] = entry;
    __asm volatile("popf");
}

void X86IdtInit() {
    memset(idt, 0, sizeof(idt));
    idtr.base = (uint32_t) idt;
    idtr.limit = sizeof(idt);
    SetIdtr(&idtr);
    PortWrite8(0x20, 0x11);
    PortWrite8(0x21, IRQ_BASE);
    PortWrite8(0x21, 4);
    PortWrite8(0x21, 1);
    PortWrite8(0xA0, 0x11);
    PortWrite8(0xA1, IRQ_BASE + 8);
    PortWrite8(0xA1, 2);
    PortWrite8(0xA1, 1);
    __asm volatile("sti");
}
