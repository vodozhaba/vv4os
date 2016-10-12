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
    bool allocated : 1;
    uint8_t available : 2;
    uint32_t page_frame_addr : 20;
} __attribute__((packed)) PageTableEntry;

typedef struct {
    uint32_t proc_id;
    uint32_t base : 20;
    uint16_t reserved : 11;
    bool used : 1;
} __attribute__((packed)) PageDirectoryDesc;

typedef struct PageDirectoryStorageStruct {
    PageDirectoryDesc page_dirs[511]; // Not a typo, we need space for pointers
    struct PageDirectoryStorageStruct* prev;
    struct PageDirectoryStorageStruct* next;
} __attribute__((packed)) PageDirectoryStorage;

typedef union {
    void* linear;
    struct {
        uint16_t offset : 12;
        uint16_t page : 10;
        uint16_t table : 10;
    } page;
} __attribute__((packed)) VirtualAddr;

static PageDirectoryStorage* head = NULL;

static PageDirectoryEntry* NewPageDirectory() {
    PageDirectoryEntry* directory = PhysAllocateFrame();
    for(size_t i = 0; i < 1024; i++) {
        PageDirectoryEntry* entry = &directory[i];
        entry->present = true;
        entry->writable = true;
        entry->user = true;
        entry->write_through_caching = true;
        entry->accessed = false;
        entry->zero = 0;
        entry->big_pages = false;
        entry->ignored = 0;
        PageTableEntry* table = PhysAllocateFrame();
        memset(table, 0, FRAME_SIZE);
        entry->page_table_addr = (uint32_t) table >> 12;
    }
    return directory;
}

static void DestroyPageDirectory(PageDirectoryEntry* directory) {
    for(uint32_t i = 0; i < 1024; i++) {
        PageDirectoryEntry* entry = &directory[i];
        PageTableEntry* table = (PageTableEntry*)(entry->page_table_addr << 12);
        X86PhysFreeFrame(table);
    }
    X86PhysFreeFrame(directory);
}

static PageDirectoryEntry* GetPageDirectory(uint32_t proc_id) {
    for(PageDirectoryStorage* storage = head; storage != NULL;
            storage = storage->next) {
        for(uint32_t i = 0; i < 511; i++) {
            PageDirectoryDesc* desc = &storage->page_dirs[i];
            if(desc->proc_id == proc_id) {
                return (PageDirectoryEntry*)(desc->base << 12);
            }
        }
    }
    return NULL;
}

static void CreateNewStorage() {
    PageDirectoryStorage* storage = PhysAllocateFrame();
    head->prev = storage;
    storage->next = head;
    storage->prev = NULL;
    head = storage;
}

static PageDirectoryDesc* FindUnusedPageDirectoryDesc() {
    for(PageDirectoryStorage* storage = head; storage != NULL;
            storage = storage->next) {
        for(uint32_t i = 0; i < 511; i++) {
            PageDirectoryDesc* desc = &storage->page_dirs[i];
            if(!desc->used) {
                return desc;
            }
        }
    }
    return NULL;
}

static PageTableEntry* GetPageTableEntry(void* virtual,
        PageDirectoryEntry* directory) {
    VirtualAddr vaddr = (VirtualAddr) virtual;
    PageDirectoryEntry* dir_entry = &directory[vaddr.page.table];
    PageTableEntry* table = (PageTableEntry*)(dir_entry->page_table_addr << 12);
    PageTableEntry* table_entry = &table[vaddr.page.page];
    return table_entry;
}

static void MapFrame(void* virtual, void* physical,
        PageDirectoryEntry* directory) {
    PageTableEntry* table_entry = GetPageTableEntry(virtual, directory);
    table_entry->present = true;
    table_entry->dirty = false;
    table_entry->global = false;
    table_entry->allocated = true;
    table_entry->page_frame_addr = (uint32_t) physical >> 12;
}

static void* FindFreeRange(uint32_t frames, PageDirectoryEntry* directory) {
    if(frames == 0) {
        return NULL;
    }
    uint32_t found = 0;
    for(void* addr = NULL; addr <= (void*) 0xFFFFF000; addr += FRAME_SIZE) {
        PageTableEntry* entry = GetPageTableEntry(addr, directory);
        if(!entry->allocated) {
            found++;
            if(found == frames) {
                return addr - (frames - 1) * FRAME_SIZE;
            }
        }
    }
    return NULL;
}

static void MarkRange(void* base, uint32_t frames, bool allocated,
        PageDirectoryEntry* directory) {
    for(void* frame = base; frame < base + frames * FRAME_SIZE;
            frame += FRAME_SIZE) {
        PageTableEntry* entry = GetPageTableEntry(frame, directory);
        entry->allocated = allocated;
    }
}

static void* AllocateRange(uint32_t frames, PageDirectoryEntry* directory) {
    void* base = FindFreeRange(frames, directory);
    MarkRange(base, frames, true, directory);
    return base;

}

// Allocates it at given base in first directory and at free range in second
// and returns it base in second
static void* AllocateRangeInTwoDirectories(void* fixed_base, uint32_t frames,
        PageDirectoryEntry* fixed_base_dir, PageDirectoryEntry* var_base_dir) {
    MarkRange(fixed_base, frames, true, fixed_base_dir);
    void* var_base = AllocateRange(frames, var_base_dir);
    for(uint32_t frame = 0; frame < frames; frame++) {
        void* phys = PhysAllocateFrame();
        uint32_t offset = frame * FRAME_SIZE;
        MapFrame(fixed_base + offset, phys, fixed_base_dir);
        MapFrame(var_base + offset, phys, fixed_base_dir);
    }
    return var_base;
}

// 'Window' = two ranges in two directories pointing to the same physical range
// Returns base in kernel directory

void* X86CreateKernelAndProcWindow(void* proc_base, uint32_t frames,
        uint32_t proc_id) {
    PageDirectoryEntry* kernel_directory = GetPageDirectory(0);
    PageDirectoryEntry* proc_directory = GetPageDirectory(proc_id);
    return AllocateRangeInTwoDirectories(proc_base, frames, proc_directory,
            kernel_directory);
}

void X86NewVirtMemTable(uint32_t proc_id) {
    PageDirectoryEntry* directory = NewPageDirectory();
    PageDirectoryDesc* desc = FindUnusedPageDirectoryDesc();
    if(desc == NULL) {
        CreateNewStorage();
        desc = head->page_dirs;
    }
    desc->used = true;
    desc->proc_id = proc_id;
    desc->base = (uint32_t) directory >> 12;
}

int X86DestroyVirtMemTable(uint32_t proc_id) {
    PageDirectoryEntry* directory = GetPageDirectory(proc_id);
    if(directory == NULL) {
        return -1;
    }
    DestroyPageDirectory(directory);
    return 0;
}

void X86VirtMemMgrInit() {

}
