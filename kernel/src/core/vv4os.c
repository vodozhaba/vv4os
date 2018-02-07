// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       core/vv4os.c
// Author:     vodozhaba
// Created on: Aug 20, 2016
// Purpose:    The main file of the project, connects all the modules.        

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "boot/multiboot/multiboot.h"
#include "core/config.h"
#include "debug/gdb_stub.h"
#include "dt/desc_tables.h"
#include "io/disk/disk.h"
#include "io/pci.h"
#include "io/ports.h"
#include "io/uart.h"
#include "io/vga_terminal.h"
#include "mem/kernel_mem.h"
#include "mem/phys_mem_mgr.h"
#include "mem/virt_mem_mgr.h"
#include "util/bitmap.h"

#define MAX_CONF_SIZE 4096
#define CONF_SIGNATURE "notjunk"

size_t available_ram;

char config[MAX_CONF_SIZE];

bool ParseConfig() {
    config[strlen(CONF_SIGNATURE)] = '\0';
    return strcmp(config, CONF_SIGNATURE) == 0;
}

void main(MultibootInformation* mi) {
    MultibootModuleStructure conf = ((MultibootModuleStructure*) mi->mods_addr)[0];
    size_t conf_size = conf.mod_end - conf.mod_start;
    memcpy(config, (void*) conf.mod_start, conf_size > MAX_CONF_SIZE ? MAX_CONF_SIZE : conf_size);
    VgaTerminalInit();
    #if defined(DEBUG)
    printf("Kernel loaded at 0x%X - 0x%X\n", KERNEL_STATIC_MEM_START, KERNEL_STATIC_MEM_END);
    #endif
    available_ram = (mi->mem_upper + 1024) << 10;
    printf("Detected %dMiB of RAM\n", available_ram / 1048576);
    if(mi->mods_count != 1) {
        VgaTerminalSwitchColorScheme(err_color_scheme);
        printf("Something's wrong with the boot modules. Aborting");
        exit(1);
    }
    printf("Initializing descriptor tables...");
    DescTablesInit();
    printf(" OK\n");
    printf("Initializing UART (baudrate %d)...", UART_DEFAULT_FREQ);
    UartInit(UART_DEFAULT_FREQ);
    printf(" OK\n");
    printf("Initializing physical memory manager...");
    PhysMemMgrInit(available_ram);
    printf(" OK\n");
    printf("Initializing virtual memory manager...");
    VirtMemMgrInit();
    printf(" OK\n");
#if defined(DEBUG)
#if defined(UART_DEBUGGING)
    printf("This build supports debugging over UART.\n");
#endif
    GdbStubInit();
#endif
    printf("Initializing I/O port manager...");
    printf(" OK\n");
    IoPortMgrInit();
    printf("Initializing PCI...");
    PciInit();
    printf(" OK\n");
    printf("Parsing boot.cfg...");
    if(ParseConfig())
        printf(" OK\n");
    else {
        VgaTerminalSwitchColorScheme(err_color_scheme);
        printf(" ERROR. Is there a valid /sys/vv4os/boot.cfg?\n");
        exit(1);
    }
    printf("Initializing disk subsystem...");
    DiskSubsystemInit();
    printf(" OK\n");
    printf("Successful boot-up.\n");
    while(true);
}
