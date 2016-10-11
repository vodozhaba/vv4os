/******************************************************************************
 * File:       /vv4os/arch/x86/dt/gdt.h                                       *
 * Author:     velikiyv4                                                      *
 * Created on: Oct 1, 2016                                                    *
 * Purpose:    Allows working with x86 Global Descriptor Table.               *
 ******************************************************************************/

#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t rpl : 2;
    bool ldt : 1;
    uint16_t index : 13;
} __attribute__((packed)) GdtSelector;

void X86GdtInit();
