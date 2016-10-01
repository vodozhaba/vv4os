/******************************************************************************
 * File:       /vv4os/arch/x86/dt/gdt.c                                       *
 * Author:     velikiyv4                                                      *
 * Created on: Oct 1, 2016                                                    *
 * Purpose:    Allows working with x86 Global Descriptor Table.               *
 ******************************************************************************/

#include "gdt_x86.h"

extern void SetGdtr(Gdtr* gdtr);

static GdtEntry gdt[8192];
static Gdtr gdtr;

static void UpdateGdt(GdtEntry* gdt, uint16_t entries) {
    gdtr.base = (uint32_t) gdt;
    gdtr.size = entries * sizeof(GdtEntry) - 1;
    SetGdtr(&gdtr);
}

static void SetGdtEntry(uint16_t n, uint32_t base, uint32_t limit, uint8_t
        access_byte, uint8_t granularity) {
    gdt[n].granularity = granularity & 0xF0;
    gdt[n].access_byte = access_byte;
    gdt[n].base_low = base;
    gdt[n].base_high = base >> 24;
    gdt[n].limit_low = limit;
    gdt[n].granularity |= (limit >> 16) & 0x0F;
}

void X86GdtInit() {
    GdtEntry null_entry = { };
    gdt[0] = null_entry;
    SetGdtEntry(1, 0, 0xFFFFF, GDT_ENTRY_ACCESS_READABLE_CODE_RING0,
            GDT_ENTRY_GRANULARITY_PAGE_32_BIT);
    SetGdtEntry(2, 0, 0xFFFFF, GDT_ENTRY_ACCESS_RW_DATA_RING0,
            GDT_ENTRY_GRANULARITY_PAGE_32_BIT);
    SetGdtEntry(3, 0, 0xFFFFF, GDT_ENTRY_ACCESS_READABLE_CODE_RING3,
            GDT_ENTRY_GRANULARITY_PAGE_32_BIT);
    SetGdtEntry(4, 0, 0xFFFFF, GDT_ENTRY_ACCESS_RW_DATA_RING3,
            GDT_ENTRY_GRANULARITY_PAGE_32_BIT);
    UpdateGdt(gdt, 5);
}
