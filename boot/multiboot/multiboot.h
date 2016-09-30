/******************************************************************************
 * File:       /vv4os/boot/multiboot/multiboot.h                              *
 * Author:     velikiyv4                                                      *
 * Created on: Sep 30, 2016                                                   *
 * Purpose:    Allows working with Multiboot-compliant bootloaders.           *
 ******************************************************************************/

#pragma once

#include <stdint.h>

typedef struct {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    union {
        struct {
            uint32_t tabsize;
            uint32_t strsize;
            uint32_t addr;
            uint32_t zero;
        } syms;
        struct {
            uint32_t num;
            uint32_t size;
            uint32_t addr;
            uint32_t shndx;
        } header;
    } elf_data;
    uint32_t mmap_length;
    uint32_t mmap_addr;
    uint32_t drives_length;
    uint32_t drives_addr;
    uint32_t config_table;
    uint32_t bootloader_name;
    uint32_t apm_table;
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;
} __attribute__((packed)) MultibootInformation;

typedef struct {
    uint32_t size;
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
} __attribute__((packed)) MultibootMemoryMap;

typedef struct {
    uint32_t size;
    uint8_t drive_number;
    uint8_t drive_mode;
    uint16_t drive_cylinders;
    uint8_t drive_heads;
    uint8_t drive_sectors;
    uint16_t drive_ports[0];
} __attribute__((packed)) MultibootDriveStructure;

typedef struct {
    uint16_t version;
    uint16_t cseg;
    uint32_t offset;
    uint16_t cseg_16;
    uint16_t dseg;
    uint16_t flags;
    uint16_t cseg_len;
    uint16_t cseg_16_len;
    uint16_t dseg_len;
} __attribute__((packed)) MultibootApmTable;
