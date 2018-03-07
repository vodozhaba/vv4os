// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       io/disk/disk.c
// Author:     vodozhaba
// Created on: Apr 24, 2017
// Purpose:    Controls the disk subsystem.                                   

#include "io/disk/disk.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include "core/config.h"
#include "io/disk/fs/fat.h"
#include "io/disk/ide.h"
#include "io/disk/volume.h"
#include "io/pci.h"
#include "util/bitmap.h"

#define PCI_SUBCLASS_SCSI_BUS_CONTROLLER 0x00
#define PCI_SUBCLASS_IDE_CONTROLLER 0x01
#define PCI_SUBCLASS_FLOPPY_DISK_CONTROLLER 0x02
#define PCI_SUBCLASS_IPI_BUS_CONTROLLER 0x03
#define PCI_SUBCLASS_RAID_CONTROLLER 0x04
#define PCI_SUBCLASS_ATA_CONTROLLER 0x05
#define PCI_SUBCLASS_SATA_CONTROLLER 0x06
#define PCI_SUBCLASS_SAS_CONTROLLER 0x07
#define PCI_SUBCLASS_OTHER_MASS_STORAGE_CONTROLLER 0x80

#ifdef DISK_INDICES_START_FROM_ZERO
#define FIRST_IX 0
#else
#define FIRST_IX 1
#endif

static DiskDescriptor* disks = NULL;
static Bitmap bitmap;
static uint8_t bitmap_data[BITMAP_SIZE(MAX_DISKS + FIRST_IX, FIRST_IX)];

static void AddDisks(DiskDescriptor* chain) {
    if(chain == NULL) {
        return;
    }
    DiskDescriptor* disk;
    Volume* volumes = NULL;
    for(disk = chain; disk; disk = disk->next) {
        disk->id = AllocateInBitmap(&bitmap, 1);
        for(uint32_t partition = 0; partition < 4; partition++) {
            if(GetPartitionEntry(disk, partition).lba_length != 0) {
                Volume* volume = FatMountVolume(disk, partition);
                volume->next = volumes;
                volumes = volume;
            }
        }
    }
    AddVolumes(volumes);
    if(disks == NULL) {
        disks = chain;
        return;
    }
    disk->next = chain;
}

DiskDescriptor* GetDiskDescriptor(uint32_t id) {
    DiskDescriptor* disk;
    for(disk = disks; disk && disk->id != id; disk = disk->next);
    return disk;
}

void* DiskReadSector(DiskDescriptor* disk, Lba48 sector, void* buf) {
    return disk->read_op(disk, sector, buf);
}

void DiskSubsystemInit() {
    bitmap.start = bitmap_data;
    bitmap.len = MAX_DISKS + FIRST_IX;
    bitmap.dead_zone = FIRST_IX;
    InitBitmap(&bitmap, false);
    for(PciDevice* controller = pci_devices_list; controller != NULL; controller = controller->next) {
	    uint8_t class_code = PciConfigReadFromDevice8(controller, PCI_CONFIG_COMMON_OFFSET_CLASS_CODE);
	    if(class_code == PCI_CLASS_CODE_MASS_STORAGE_CONTROLLER) {
    	    uint8_t subclass = PciConfigReadFromDevice8(controller, PCI_CONFIG_COMMON_OFFSET_SUBCLASS);
    	    switch(subclass) {
                case PCI_SUBCLASS_IDE_CONTROLLER:
                    AddDisks(IdeControllerInit(controller));
                    break;
                default:
                    printf("Detected an unsupported mass storage controller.\nSome drives might be absent.\n");
    	    }
	    }
    }
}

MbrPartitionEntry GetPartitionEntry(DiskDescriptor* disk, uint32_t partition) {
    assert(partition < 4);
    Mbr* mbr = malloc(sizeof(*mbr));
    disk->read_op(disk, 0, mbr);
    MbrPartitionEntry ret = mbr->partition_table[partition];
    free(mbr);
    return ret;
}

void* PartitionReadSector(DiskDescriptor* disk, uint32_t partition, Lba48 sector, void* buf) {
    Lba48 base = GetPartitionEntry(disk, partition).start_lba;
    return disk->read_op(disk, base + sector, buf);
}

void* PartitionRead(DiskDescriptor* disk, uint32_t partition, uint64_t addr, uint64_t length, void* buf) {
    void* sector_buf = malloc(DISK_SECTOR_SIZE);
    void* ret = buf;
    size_t padding = addr % DISK_SECTOR_SIZE;
    size_t leading = DISK_SECTOR_SIZE - padding;
    if(!PartitionReadSector(disk, partition, addr / DISK_SECTOR_SIZE, sector_buf)) {
        return NULL;
    }
    memcpy(buf, sector_buf + padding, leading);
    addr += leading;
    buf += leading;
    length -= leading;
    Lba48 base = addr / DISK_SECTOR_SIZE, full_sectors = length / DISK_SECTOR_SIZE;
    for(size_t i = base; i < base + full_sectors; i++, buf += DISK_SECTOR_SIZE) {
        if(!PartitionReadSector(disk, partition, i, buf)) {
            return NULL;
        }
    }
    length -= full_sectors * DISK_SECTOR_SIZE;
    if(!PartitionReadSector(disk, partition, base + full_sectors, sector_buf)) {
        return NULL;
    }
    memcpy(buf, sector_buf, length);
    free(sector_buf);
    return ret;
}
