// File:       io/disk/disk.h
// Author:     vodozhaba
// Created on: Apr 24, 2017
// Purpose:    Controls the disk subsystem.

#pragma once

#include <assert.h>
#include <stdint.h>

typedef struct {
    uint8_t head;
    uint8_t sector : 6;
    uint16_t cylinder : 10;
} __attribute__((packed)) Chs;
typedef uint64_t Lba48;

struct DiskDescriptor;
typedef struct DiskDescriptor DiskDescriptor;

typedef void* (*DiskReadOp)(DiskDescriptor* disk, Lba48 sector, void* buf);

struct DiskDescriptor {
    void* data;
    struct DiskDescriptor* next;
    DiskReadOp read_op;
    uint32_t id;
};

void DiskSubsystemInit();
void* DiskReadSector(uint32_t id, Lba48 sector, void* buf);
void* PartitionReadSector(uint32_t disk_id, uint32_t partition, Lba48 sector, void* buf);
