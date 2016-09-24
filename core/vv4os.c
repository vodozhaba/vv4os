/******************************************************************************
 * File:       /vv4os/core/vv4os.c                                            *
 * Author:     velikiyv4                                                      *
 * Created on: Aug 20, 2016                                                   *
 * Purpose:    The main file of the project, connects all the modules.        *
 ******************************************************************************/

#include "arch/x86/gdt.h"
#include "io/vga_terminal.h"
#include "stdlib/stdio.h"

static void InitAll() {
    VgaTerminalInit();
    printf("Initialized VGA terminal\n");
    InitGdt();
    printf("Initialized GDT\n");
}

void main() {
    InitAll();
}
