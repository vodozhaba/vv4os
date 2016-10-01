/******************************************************************************
 * File:       /vv4os/arch/x86/dt/gdt.h                                       *
 * Author:     velikiyv4                                                      *
 * Created on: Oct 1, 2016                                                    *
 * Purpose:    Allows working with x86 Global Descriptor Table.               *
 ******************************************************************************/

#pragma once

#include <stdbool.h>
#include <stdint.h>

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

void X86GdtInit();
