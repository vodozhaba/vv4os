/******************************************************************************
 * File:       /vv4os/arch/x86/dt/desc_tables_x86.c                           *
 * Author:     vodozhaba                                                      *
 * Created on: Oct 9, 2016                                                    *
 * Purpose:    Allows setting up all tables specific to the x86 architecture. *
 ******************************************************************************/

#include "gdt_x86.h"
#include "idt_x86.h"

void X86DescTablesInit() {
    X86GdtInit();
    X86IdtInit();
}
