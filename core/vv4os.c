/******************************************************************************
 * File:       /vv4os/core/vv4os.c                                            *
 * Author:     velikiyv4                                                      *
 * Created on: Aug 20, 2016                                                   *
 * Purpose:    The main file of the project, connects all the modules.        *
 ******************************************************************************/

#include <stdbool.h>
#include <stddef.h>
#include "boot/multiboot/multiboot.h"
#include "dt/desc_tables.h"
#include "io/vga_terminal.h"
#include "mem/phys_mem_mgr.h"
#include "stdlib/stdio.h"

size_t available_ram;

static void InitAll() {
    VgaTerminalInit();
    printf("Initialized VGA terminal\n");
    DescTablesInit();
    printf("Initialized processor descriptor tables\n");
    printf("Detected %dMiB of RAM\n", available_ram / 1048576);
    PhysMemMgrInit(available_ram);
    printf("Initialized physical memory manager");
}

void main(MultibootInformation* mi) {
    available_ram = (mi->mem_upper + 1024) << 10;
    InitAll();
    while(true);
}
