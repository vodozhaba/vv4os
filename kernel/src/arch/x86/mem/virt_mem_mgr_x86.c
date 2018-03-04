// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       arch/x86/mem/virt_mem_mgr_x86.c
// Author:     vodozhaba
// Created on: Oct 11, 2016
// Purpose:    Translates virtual memory to physical and (de)allocates it.

#include "mem/virt_mem_mgr.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "arch/x86/dt/idt_x86.h"
#include "core/config.h"
#include "io/vga_terminal.h"
#include "mem/kernel_mem.h"
#include "mem/phys_mem_mgr.h"
#include "user/process.h"

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

extern char _kernel_page_directory[];
extern char _kernel_page_table[];

static PageDirectoryEntry* kernel_page_directory = (PageDirectoryEntry*) _kernel_page_directory;
static PageTableEntry* kernel_page_table = (PageTableEntry*) _kernel_page_table;
static PageDirectoryEntry* current_page_directory;

static void* AllocateMap(void* phys);
static void UnmapFrame(void* virtual, PageDirectoryEntry* directory);

static PageTableEntry GetPte(VirtualAddr vaddr, PageDirectoryEntry* directory) {
    if(directory == kernel_page_directory) {
        return kernel_page_table[vaddr.page.directory_entry * 1024 + vaddr.page.table_entry];
    }
    PageDirectoryEntry pde = directory[vaddr.page.directory_entry];
    if(pde.present == false) {
        PageTableEntry ret = { 0 };
        return ret;
    }
    void* phys = (void*)(pde.page_table_addr << 12);
    PageTableEntry* table = AllocateMap(phys);
    PageTableEntry pte = table[vaddr.page.table_entry];
    UnmapFrame(table, kernel_page_directory);
    return pte;
}

static void CreatePageTable(VirtualAddr vaddr,
        PageDirectoryEntry* directory) {
    void* phys = PhysAllocateFrame();
    void* virt = AllocateMap(phys);
    memset(virt, 0, FRAME_SIZE);
    UnmapFrame(virt, kernel_page_directory);
    PageDirectoryEntry pde = { 0 };
    pde.writable = true;
    pde.user = true;
    pde.page_table_addr = (uint32_t) phys >> 12;
    pde.present = true;
    directory[vaddr.page.directory_entry] = pde;
}

static void SetPte(VirtualAddr vaddr, PageDirectoryEntry* directory, PageTableEntry pte) {
    if(directory == kernel_page_directory) {
        kernel_page_table[vaddr.page.directory_entry * 1024 + vaddr.page.table_entry] = pte;
        return;
    }
    PageDirectoryEntry* pde = &directory[vaddr.page.directory_entry];
    if(pde->present == false) {
        CreatePageTable(vaddr, directory);
    }
    void* phys = (void*)(pde->page_table_addr << 12);
    PageTableEntry* table = AllocateMap(phys);
    table[vaddr.page.table_entry] = pte;
    UnmapFrame(table, kernel_page_directory);
    // TODO: add conditional INVLPG for post-i486
    uint32_t cr3;
    __asm volatile("movl %%cr3, %d0" : "=a" (cr3));
    __asm volatile("movl %d0, %%cr3" : : "a" (cr3));
}

static void MapFrame(void* virtual, void* physical, bool writable, bool cached,
        bool user, PageDirectoryEntry* directory) {
    VirtualAddr vaddr = (VirtualAddr) virtual;
    if(vaddr.page.directory_entry == 0 && vaddr.page.table_entry == 0) {
        return;
    }
    PageTableEntry pte;
    pte.writable = writable;
    pte.user = user;
    pte.write_through_caching = false;
    pte.cache_disable = !cached;
    pte.dirty = false;
    pte.zero = 0;
    pte.global = false;
    pte.ignored = 0;
    pte.available = 0;
    pte.page_frame_addr = (uint32_t) physical >> 12;
    pte.present = true;
    SetPte((VirtualAddr) virtual, directory, pte);
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

static bool IsMapped(void* frame, PageDirectoryEntry*
        directory) {
    PageTableEntry pte = GetPte((VirtualAddr) frame, directory);
    return pte.present;
}

static void* FindContiguousFreeFrames(uint32_t frames,
        PageDirectoryEntry* directory, bool kernel) {
    uint32_t found = 0;
    //   1111
    //   FFFFF000
    //  +00001000
    // 1|00000000
    // One gets truncated, frame = NULL
    for(void* frame = (kernel ? KERNEL_STATIC_MEM_START : 0) + 0x1000; frame != NULL; frame += 0x1000) {
        if(IsMapped(frame, directory)) {
            found = 0;
        } else if(++found == frames) {
            return frame - frames * 0x1000 + 0x1000;
        }
    }
    return NULL;
}

static void UnmapFrame(void* virtual, PageDirectoryEntry* directory) {
    PageTableEntry pte = { 0 };
    SetPte((VirtualAddr) virtual, directory, pte);
}

void X86PageFaultHandler(X86CpuState* cpu_state) {
    uint32_t pid = UserProcessCurrent();
    char* action;
    switch(cpu_state->error_code) {
        case 0:
        case 4:
            action = "read a non-present page";
            break;
        case 1:
        case 5:
            action = "read a protected page";
            break;
        case 2:
        case 6:
            action = "write to a non-present page";
            break;
        case 3:
        case 7:
            action = "write to a protected page";
            break;
        default:
            action = "do something bad";
    }
    fprintf(stderr, "Process with PID %d attempted to %s and was terminated.\n", pid, action);
    RemoveProcess(pid);
}

static void* AllocateMap(void* phys) {
    void* virt = FindContiguousFreeFrames(1, kernel_page_directory, true);
    MapFrame(virt, phys, true, true, false, kernel_page_directory);
    return virt;
}

void X86SwitchPageDirectory(PageDirectoryEntry* directory) {
    current_page_directory = directory;
    if(directory == kernel_page_directory) {
        __asm volatile("movl %d0, %%cr3" : : "a" ((void*) kernel_page_directory - KERNEL_STATIC_MEM_START));
    } else {
        VirtualAddr vaddr = (VirtualAddr)(void*) directory;
        PageTableEntry pte = kernel_page_table[vaddr.page.directory_entry * 1024 + vaddr.page.table_entry];
        uint32_t cr3 = pte.page_frame_addr << 12;
        __asm volatile("movl %d0, %%cr3" : : "a" (cr3));
    }
}

static PageDirectoryEntry* MapProcess(void* k_base, uint32_t frames) {
    void* phys = PhysAllocateFrame();
    PageDirectoryEntry* dir = AllocateMap(phys);
    memset(dir, 0, 1024 * sizeof(PageDirectoryEntry));
    for(uint32_t i = 0; i < frames; i++) {
        PageTableEntry pte = GetPte((VirtualAddr)(k_base + i * FRAME_SIZE), kernel_page_directory);
        pte.writable = true;
        pte.user = true;
        SetPte((VirtualAddr)(USER_PROCESS_BASE + i * FRAME_SIZE), dir, pte);
    }
    uint32_t first_kernel_pde = (uint32_t) KERNEL_STATIC_MEM_START >> 22;
    for(size_t i = first_kernel_pde; i < 1024; i++) {
        dir[i] = kernel_page_directory[i];
    }
    return dir;
}

void X86VirtMemMgrInit() {
    X86RegisterIsrHandler(14, X86PageFaultHandler);
    memset(kernel_page_directory, 0, 1024 * sizeof(PageDirectoryEntry));
    memset(kernel_page_table, 0, 1048576 * sizeof(PageTableEntry));
    for(size_t i = 0; i < 1024; i++) {
        PageDirectoryEntry* entry = &kernel_page_directory[i];
        memset(entry, 0, sizeof(*entry));
        entry->user = true;
        entry->cache_disable = true;
        entry->write_through_caching = true;
        entry->writable = true;
        entry->present = true;
        entry->page_table_addr = ((uint32_t) &kernel_page_table[i * 1024] - (uint32_t) KERNEL_STATIC_MEM_START) >> 12;
    }
    MapRange((void*) 0x1000, (void*) 0x1000, 1048576 / FRAME_SIZE - 1, true, false, false, kernel_page_directory);
    uint32_t first_kernel_frame = (uint32_t) KERNEL_STATIC_MEM_START & 0xFFC00000;
    uint32_t kernel_len = (uint32_t) KERNEL_STATIC_MEM_END - first_kernel_frame;
    uint32_t kernel_frames = kernel_len / FRAME_SIZE;
    if(kernel_len % FRAME_SIZE) {
        kernel_frames++;
    }
    MapRange((void*) first_kernel_frame + 0x1000, (void*) 0x1000, kernel_frames, true, true, false, kernel_page_directory);
    X86SwitchPageDirectory(kernel_page_directory);
}

void* X86AllocateContiguousVirtualFrames(uint32_t frames, bool kernel) {
    void* base = FindContiguousFreeFrames(frames, current_page_directory, kernel);
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
        PageTableEntry pte = GetPte((VirtualAddr) addr, current_page_directory);
        void* phys = (void*)(pte.page_frame_addr << 12);
        if(pte.present) {
            if(phys) {
                PhysFreeFrame(phys);
            }
            UnmapFrame(addr, current_page_directory);
        }
    }
}

void* X86CreateAddressSpace(void* k_base, size_t frames) {
    PageDirectoryEntry* ret = MapProcess(k_base, frames);
    for(size_t i = 0; i < frames; i++) {
        UnmapFrame(k_base + i * FRAME_SIZE, kernel_page_directory);
    }
    return ret;
}

void* X86CreateStack(void* address_space, void* top, size_t size) {
    PageDirectoryEntry* dir = address_space;
    uint32_t frames = size / FRAME_SIZE;
    if(size % FRAME_SIZE) {
        frames++;
    }
    void* bottom = top - frames * FRAME_SIZE;
    for(uint32_t i = 0; i < frames; i++) {
        void* phys = PhysAllocateFrame();
        if(!phys) {
            return NULL;
        }
        MapFrame(bottom + i * FRAME_SIZE, phys, true, true, true, dir);
    }
    return (void*)((uint32_t) top & 0xFFFFF000);
}

void X86DeleteAddressSpace(void* address_space) {
    for(void* frame = NULL; frame < (void*)((uint32_t) KERNEL_STATIC_MEM_START & 0xFFC00000); frame += 0x1000) {
        PageTableEntry pte = GetPte((VirtualAddr) frame, address_space);
        if(pte.present) {
            UnmapFrame(frame, address_space);
            void* phys = (void*)(pte.page_frame_addr << 12);
            if(phys) {
                PhysFreeFrame(phys);
            }
        }
    }
}

void X86SwitchAddressSpace(void* address_space) {
    if(address_space) {
        X86SwitchPageDirectory(address_space);
    } else {
        X86SwitchAddressSpace(kernel_page_directory);
    }
}