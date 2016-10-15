/******************************************************************************
 * File:       /vv4os/arch/x86/mem/virt_mem_mgr_x86.c                         *
 * Author:     velikiyv4                                                      *
 * Created on: Oct 11, 2016                                                   *
 * Purpose:    Translates virtual memory to physical and (de)allocates it.    *
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "mem/phys_mem_mgr.h"
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

static PageTableEntry* GetPageTableEntryPtr(VirtualAddr vaddr,
        PageDirectoryEntry* directory) {
    PageDirectoryEntry pde = directory[vaddr.page.directory_entry];
    if(pde.present == false) {
        return NULL;
    }
    PageTableEntry* pte = (PageTableEntry*)(pde.page_table_addr << 12);
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
    PageTableEntry* pte = GetPageTableEntryPtr(vaddr, directory);
    if(pte == NULL) {
        pte = CreatePageTable(vaddr, directory);
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
    __asm volatile("movl %d0, %%cr3" : : "a" (directory));
}

static PageDirectoryEntry* CreatePageDirectory() {
    PageDirectoryEntry* directory = PhysAllocateFrame();
    memset(directory, 0, FRAME_SIZE);
    // Identity page the kernel so that it exists in all processes' memory
    MapRange(NULL, NULL, 16384, true, false, false, directory);
    return directory;
}

static inline void EnablePaging() {
    uint32_t cr0;
    __asm volatile("mov %%cr0, %d0" : "=a" (cr0) : );
    cr0 |= 0x80000000;
    __asm volatile("mov %d0, %%cr0" : : "a" (cr0));
}

void X86VirtMemMgrInit() {
    kernel_page_directory = CreatePageDirectory();
    SwitchPageDirectory(kernel_page_directory);
    EnablePaging();
}
