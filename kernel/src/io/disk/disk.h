// File:       io/disk/disk.h
// Author:     vodozhaba
// Created on: Apr 24, 2017
// Purpose:    Controls the disk subsystem.

#pragma once

#include <stdint.h>

// Drivers for disks where sectors are not 512 bytes long or don't exist should emulate this
#define DISK_SECTOR_SIZE 512

typedef struct {
    uint8_t head;
    uint8_t sector : 6;
    uint16_t cylinder : 10;
} __attribute__((packed)) Chs;
typedef uint64_t Lba48;

typedef struct DiskDescriptor {
    void* data;
    struct DiskDescriptor* next;
    void* (*read_op)(struct DiskDescriptor* disk, Lba48 sector, void* buf);
    uint32_t id;
} DiskDescriptor;

typedef struct {
    uint8_t active;
    Chs start_chs;
    uint8_t system_id;
    Chs end_chs;
    Lba48 start_lba : 32;
    Lba48 lba_length : 32;
} __attribute__((packed)) MbrPartitionEntry;

typedef struct {
    uint8_t bootstrap[436];
    uint8_t uuid [10];
    MbrPartitionEntry partition_table[4];
    uint16_t signature;
} __attribute__((packed)) Mbr;

void DiskSubsystemInit();
DiskDescriptor* GetDiskDescriptor(uint32_t id);
void* DiskReadSector(DiskDescriptor* disk, Lba48 sector, void* buf);
MbrPartitionEntry GetPartitionEntry(DiskDescriptor* disk, uint32_t partition);
void* PartitionReadSector(DiskDescriptor* disk, uint32_t partition, Lba48 sector, void* buf);
void* PartitionRead(DiskDescriptor* disk, uint32_t partition, uint64_t addr, uint64_t length, void* buf);
