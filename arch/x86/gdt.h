/******************************************************************************
 * File:       /vv4os/arch/x86/gdt.h                                          *
 * Author:     velikiyv4                                                      *
 * Created on: Sep 24, 2016                                                   *
 * Purpose:    Allows working with the IA32 Global Descriptor Table.          *
 ******************************************************************************/

#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint32_t addr;
    uint16_t size;
} __attribute__((packed)) Gdtr;

typedef struct {
    uint8_t rpl : 2; // Requested Privilege Level
    bool local : 1;
    uint16_t offset : 13;
} __attribute__((packed)) GdtSelector;

typedef struct {
    bool accessed : 1;
    bool opt_rights : 1; // readable for code, writable for data
    bool dc : 1; // direction for data (1 = down),
                 // conforming for code (1 = can be executed from lower ring)
    bool is_code : 1;
    uint8_t one : 1;
    uint8_t dpl : 2; // Descriptor Privilege Level
    bool present : 1;
} __attribute__((packed)) GdtAccessByte;

typedef struct {
    uint16_t limit_low;
    uint32_t base_low : 24;
    GdtAccessByte access_byte;
    uint8_t limit_high : 4;
    uint8_t zero : 2;
    bool is_32bit;
    bool page_gr;
    uint8_t base_high;
} __attribute__((packed)) GdtDesc;

static inline void SetGdt(GdtDesc* gdt, uint32_t amount) {
    Gdtr gdtr = { .addr = (uintptr_t) gdt,
                  .size = amount * sizeof(GdtDesc) - 1 };
    __asm volatile("lgdt (%0)" : : "a" (&gdtr));
}

void InitGdt();
