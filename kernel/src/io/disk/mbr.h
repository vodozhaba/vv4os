// File:       io/disk/mbr.h
// Author:     vodozhaba
// Created on: Jan 03, 2017
// Purpose:    Describes the Master Boot Record structure.

#include <stdint.h>
#include "io/disk/disk.h"

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
