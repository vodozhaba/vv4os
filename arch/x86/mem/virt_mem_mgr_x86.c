/******************************************************************************
 * File:       /vv4os/arch/x86/mem/virt_mem_mgr_x86.c                         *
 * Author:     velikiyv4                                                      *
 * Created on: Oct 11, 2016                                                   *
 * Purpose:    Translates virtual memory to physical and (de)allocates it.    *
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "arch/x86/dt/idt_x86.h"
#include "io/vga_terminal.h"
#include "mem/phys_mem_mgr.h"
#include "stdlib/stdio.h"
#include "stdlib/stdlib.h"

typedef struct {
    bool present : 1;
    bool writable : 1;
    bool user : 1;
    bool write_through_caching : 1;
    bool cache_disable : 1;
    bool accessed : 1;
    uint8_t zero : 1;
    bool big_pages : 1;
    uint8_t ignored : 1;
    uint8_t available : 3;
    uint32_t page_table_addr : 20;
} __attribute__((packed)) PageDirectoryEntry;

typedef struct {
    bool present : 1;
    bool writable : 1;
    bool user : 1;
    bool write_through_caching : 1;
    bool cache_disable : 1;
    bool dirty : 1;
    uint8_t zero : 1;
    bool global : 1;
    uint8_t ignored : 1;
    bool allocated : 1;
    uint8_t available : 2;
    uint32_t page_frame_addr : 20;
} __attribute__((packed)) PageTableEntry;

void X86VirtMemMgrInit() {

}
