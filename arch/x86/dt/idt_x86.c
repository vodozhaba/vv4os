/******************************************************************************
 * File:       /vv4os/arch/x86/dt/idt_x86.c                                   *
 * Author:     velikiyv4                                                      *
 * Created on: Oct 9, 2016                                                    *
 * Purpose:    Allows working with x86 Interrupt Descriptor Table.            *
 ******************************************************************************/

#include <stdbool.h>
#include "idt_x86.h"
#include "io/ports.h"
#include "stdlib/stdio.h"
#include "stdlib/stdlib.h"

extern void X86SetIdtr(Idtr* idtr);

IdtEntry idt[256];
Idtr idtr;

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

void X86IsrHandler(__attribute__((unused)) InterruptedCpuState state) {

}

void X86SoftIntHandler(__attribute__((unused)) InterruptedCpuState state) {

}

void X86IrqHandler(InterruptedCpuState state) {
    if(state.interrupt > IRQ(7)) {
        PortWrite8(0xA0, 0x20);
        return;
    }
    PortWrite8(0x20, 0x20);
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

    __asm volatile("sti");
}
