// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       arch/x86/dt/gdt.c
// Author:     vodozhaba
// Created on: Oct 1, 2016
// Purpose:    Allows working with x86 Global Descriptor Table.

#include "gdt_x86.h"
#include <stdlib.h>

#define SCHED_STACK_SIZE 16384

#define GDT_ENTRY_ACCESS_RO_DATA_RING0                  0x90
#define GDT_ENTRY_ACCESS_RW_DATA_RING0                  0x92
#define GDT_ENTRY_ACCESS_RO_DATA_GROW_DOWN_RING0        0x94
#define GDT_ENTRY_ACCESS_RW_DATA_GROW_DOWN_RING0        0x96
#define GDT_ENTRY_ACCESS_CODE_RING0                     0x98
#define GDT_ENTRY_ACCESS_READABLE_CODE_RING0            0x9A
#define GDT_ENTRY_ACCESS_CONFORMING_CODE_RING0          0x9C
#define GDT_ENTRY_ACCESS_CONFORMING_READABLE_CODE_RING0 0x9E
#define GDT_ENTRY_ACCESS_RO_DATA_RING1                  0xA0
#define GDT_ENTRY_ACCESS_RW_DATA_RING1                  0xA2
#define GDT_ENTRY_ACCESS_RO_DATA_GROW_DOWN_RING1        0xA4
#define GDT_ENTRY_ACCESS_RW_DATA_GROW_DOWN_RING1        0xA6
#define GDT_ENTRY_ACCESS_CODE_RING1                     0xA8
#define GDT_ENTRY_ACCESS_READABLE_CODE_RING1            0xAA
#define GDT_ENTRY_ACCESS_CONFORMING_CODE_RING1          0xAC
#define GDT_ENTRY_ACCESS_CONFORMING_READABLE_CODE_RING1 0xAE
#define GDT_ENTRY_ACCESS_RO_DATA_RING2                  0xD0
#define GDT_ENTRY_ACCESS_RW_DATA_RING2                  0xD2
#define GDT_ENTRY_ACCESS_RO_DATA_GROW_DOWN_RING2        0xD4
#define GDT_ENTRY_ACCESS_RW_DATA_GROW_DOWN_RING2        0xD6
#define GDT_ENTRY_ACCESS_CODE_RING2                     0xD8
#define GDT_ENTRY_ACCESS_READABLE_CODE_RING2            0xDA
#define GDT_ENTRY_ACCESS_CONFORMING_CODE_RING2          0xDC
#define GDT_ENTRY_ACCESS_CONFORMING_READABLE_CODE_RING2 0xDE
#define GDT_ENTRY_ACCESS_RO_DATA_RING3                  0xF0
#define GDT_ENTRY_ACCESS_RW_DATA_RING3                  0xF2
#define GDT_ENTRY_ACCESS_RO_DATA_GROW_DOWN_RING3        0xF4
#define GDT_ENTRY_ACCESS_RW_DATA_GROW_DOWN_RING3        0xF6
#define GDT_ENTRY_ACCESS_CODE_RING3                     0xF8
#define GDT_ENTRY_ACCESS_READABLE_CODE_RING3            0xFA
#define GDT_ENTRY_ACCESS_CONFORMING_CODE_RING3          0xFC
#define GDT_ENTRY_ACCESS_CONFORMING_READABLE_CODE_RING3 0xFE

#define GDT_ENTRY_GRANULARITY_BYTE_16_BIT               0x00
#define GDT_ENTRY_GRANULARITY_BYTE_32_BIT               0x40
#define GDT_ENTRY_GRANULARITY_PAGE_16_BIT               0x80
#define GDT_ENTRY_GRANULARITY_PAGE_32_BIT               0xC0

typedef struct {
    uint32_t unused_1;
    uint32_t esp0;
    uint16_t ss0;
    uint16_t unused_2[46];
    uint16_t iopb;
} __attribute__((packed)) Tss;

typedef struct {
    uint16_t limit_low;
    uint32_t base_low : 24;
    uint8_t access_byte;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed)) GdtEntry;

typedef struct {
    uint16_t size;
    uint32_t base;
} __attribute__((packed)) Gdtr;

extern void X86SetGdtr(Gdtr* gdtr);

static GdtEntry gdt[8192];
static Gdtr gdtr;

static Tss tss = { 0 };

static void UpdateGdt(GdtEntry* gdt, uint16_t entries) {
    gdtr.base = (uint32_t) gdt;
    gdtr.size = entries * sizeof(GdtEntry) - 1;
    X86SetGdtr(&gdtr);
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
    __asm volatile("pushw %%ss\n\tpopw %0" : "=a" (tss.ss0));
    void* stack = malloc(SCHED_STACK_SIZE);
    memset(&tss, 0, sizeof(tss));
    tss.esp0 = (uint32_t) stack;
    tss.iopb = sizeof(tss);
    SetGdtEntry(5, (uint32_t) &tss, sizeof(tss), 0x89, GDT_ENTRY_GRANULARITY_PAGE_32_BIT);
    UpdateGdt(gdt, 6);
}
