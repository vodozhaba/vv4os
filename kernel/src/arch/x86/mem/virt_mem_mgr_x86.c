// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       arch/x86/mem/virt_mem_mgr_x86.c
// Author:     vodozhaba
// Created on: Oct 11, 2016
// Purpose:    Translates virtual memory to physical and (de)allocates it.

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "arch/x86/dt/idt_x86.h"
#include "io/vga_terminal.h"
#include "mem/phys_mem_mgr.h"

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
    uint8_t available : 3;
    uint32_t page_frame_addr : 20;
} __attribute__((packed)) PageTableEntry;

typedef union {
    struct {
        uint16_t offset : 12;
        uint16_t table_entry : 10;
        uint16_t directory_entry : 10;
    } __attribute__((packed)) page;
    void* linear;
} VirtualAddr;

static PageDirectoryEntry* kernel_page_directory;
static PageDirectoryEntry* current_page_directory;

static PageTableEntry* GetPageTableEntryPtr(VirtualAddr vaddr,
        PageDirectoryEntry* directory) {
    PageDirectoryEntry pde = directory[vaddr.page.directory_entry];
    if(pde.present == false) {
        return NULL;
    }
    PageTableEntry* table = (PageTableEntry*)(pde.page_table_addr << 12);
    PageTableEntry* pte = &table[vaddr.page.table_entry];
    return pte;
}

static PageTableEntry* CreatePageTable(VirtualAddr vaddr,
        PageDirectoryEntry* directory) {
    PageDirectoryEntry* pde = &directory[vaddr.page.directory_entry];
    PageTableEntry* table;
    if(pde->present) {
        table = (PageTableEntry*)(pde->page_table_addr << 12);
    } else {
        table = PhysAllocateFrame();
    }
    memset(table, 0, FRAME_SIZE);
    pde->writable = true;
    pde->user = true;
    pde->write_through_caching = false;
    pde->cache_disable = false;
    pde->accessed = false;
    pde->zero = 0;
    pde->big_pages = false;
    pde->ignored = 0;
    pde->available = 0;
    pde->page_table_addr = (uint32_t) table >> 12;
    pde->present = true;
    return table;
}

static void MapFrame(void* virtual, void* physical, bool writable, bool cached,
        bool user, PageDirectoryEntry* directory) {
    VirtualAddr vaddr = (VirtualAddr) virtual;
    if(vaddr.page.directory_entry == 0 && vaddr.page.table_entry == 0) {
        return;
    }
    PageTableEntry* pte = GetPageTableEntryPtr(vaddr, directory);
    if(pte == NULL) {
        PageTableEntry* table = CreatePageTable(vaddr, directory);
        pte = &table[vaddr.page.table_entry];
    }
    pte->writable = writable;
    pte->user = user;
    pte->write_through_caching = false;
    pte->cache_disable = !cached;
    pte->dirty = false;
    pte->zero = 0;
    pte->global = false;
    pte->ignored = 0;
    pte->available = 0;
    pte->page_frame_addr = (uint32_t) physical >> 12;
    pte->present = true;
}

static void MapRange(void* virt_base, void* phys_base, uint32_t frames,
        bool writable, bool cached, bool user, PageDirectoryEntry* directory) {
    for(uint32_t frame = 0; frame < frames; frame++) {
        uint32_t offset = frame * FRAME_SIZE;
        void* virtual = virt_base + offset;
        void* physical = phys_base + offset;
        MapFrame(virtual, physical, writable, cached, user, directory);
    }
}

static inline void SwitchPageDirectory(PageDirectoryEntry* directory) {
    current_page_directory = directory;
    __asm volatile("movl %d0, %%cr3" : : "a" (directory));
}

static PageDirectoryEntry* CreatePageDirectory() {
    PageDirectoryEntry* directory = PhysAllocateFrame();
    memset(directory, 0, FRAME_SIZE);
    // Identity page the kernel so that it exists in all processes' memory
    MapRange((void*) 0x1000, (void*) 0x1000, 16383, true, false, false,
            directory);
    return directory;
}

static inline void EnablePaging() {
    uint32_t cr0;
    __asm volatile("mov %%cr0, %d0" : "=a" (cr0) : );
    cr0 |= 0x80000000;
    __asm volatile("mov %d0, %%cr0" : : "a" (cr0));
}

static bool IsMapped(void* frame, PageDirectoryEntry*
        directory) {
    PageTableEntry* pte = GetPageTableEntryPtr((VirtualAddr) frame, directory);
    return pte != NULL && pte->present;
}

static void* FindContiguousFreeFrames(uint32_t frames,
        PageDirectoryEntry* directory) {
    uint32_t found = 0;
    //   1111
    //   FFFFF000
    //  +00001000
    // 1|00000000
    // One gets truncated, frame = NULL
    for(void* frame = (void*) 0x1000; frame != NULL; frame += 0x1000) {
        if(IsMapped(frame, directory)) {
            found = 0;
        } else if(++found == frames) {
            return frame - frames * 0x1000 + 0x1000;
        }
    }
    return NULL;
}

static void UnmapFrame(void* virtual, PageDirectoryEntry* directory) {
    PageTableEntry* pte = GetPageTableEntryPtr((VirtualAddr) virtual,
            directory);
    pte->present = false;
}

void X86PageFaultHandler(InterruptedCpuState* cpu_state) {
    VgaTerminalSwitchColorScheme(err_color_scheme);
    printf(
"A page fault (error code %d) has occured. It's a fatal error. Please, write down\n"
"this message and what you were doing when the error occured and submit an\n"
"issue at ", cpu_state->error_code);
    VgaTerminalSwitchColorScheme(link_color_scheme);
    printf("https://github.com/vodozhaba/vv4os/issues/\n");
    exit(1);
}

void X86VirtMemMgrInit() {
    X86RegisterIsrHandler(14, X86PageFaultHandler);
    kernel_page_directory = CreatePageDirectory();
    SwitchPageDirectory(kernel_page_directory);
    EnablePaging();
}

void* X86AllocateContiguousVirtualFrames(uint32_t frames) {
    void* base = FindContiguousFreeFrames(frames, current_page_directory);
    for(uint32_t frame = 0; frame < frames; frame++) {
        uint32_t offset = frame * 0x1000;
        void* addr = base + offset;
        MapFrame(addr, PhysAllocateFrame(), true, true, false,
                current_page_directory);
    }
    return base;
}

void X86FreeContiguousVirtualFrames(void* base, uint32_t frames) {
    for(uint32_t frame = 0; frame < frames; frame++) {
        uint32_t offset = frame * 0x1000;
        void* addr = base + offset;
        UnmapFrame(addr, current_page_directory);
    }
}
