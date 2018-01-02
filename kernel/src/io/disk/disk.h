// File:       io/disk/disk.h
// Author:     vodozhaba
// Created on: Apr 24, 2017
// Purpose:    Controls the disk subsystem.

#pragma once

#include <stdint.h>

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

void* DiskReadSector(uint32_t id, Lba48 sector, void* buf);
void DiskSubsystemInit();
