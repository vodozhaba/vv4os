// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       io/disk/disk.c
// Author:     vodozhaba
// Created on: Apr 24, 2017
// Purpose:    Controls the disk subsystem.                                   

#include <stddef.h>
#include <stdio.h>
#include "core/config.h"
#include "io/disk/ide.h"
#include "io/disk/mbr.h"
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
    for(disk = chain; disk; disk = disk->next) {
        disk->id = AllocateInBitmap(&bitmap, 1);
    }
    if(disks == NULL) {
        disks = chain;
        return;
    }
    for(disk = disks; disk->next; disk = disk->next);
    disk->next = chain;
}

static DiskDescriptor* GetDiskDescriptor(uint32_t id) {
    DiskDescriptor* disk;
    for(disk = disks; disk && disk->id != id; disk = disk->next);
    return disk;
}

void* DiskReadSector(uint32_t id, Lba48 sector, void* buf) {
    DiskDescriptor* disk = GetDiskDescriptor(id);
    return disk->read_op(disk, sector, buf);
}

void DiskSubsystemInit() {
    bitmap.start = bitmap_data;
    bitmap.len = MAX_DISKS + FIRST_IX;
    bitmap.dead_zone = FIRST_IX;
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

static Lba48 GetPartitionStart(DiskDescriptor* disk, uint32_t partition) {
    Mbr mbr;
    disk->read_op(disk, 0, &mbr);
    assert(partition < 4);
    return mbr.partition_table[partition].start_lba;
}

void* PartitionReadSector(uint32_t disk_id, uint32_t partition, Lba48 sector, void* buf) {
    DiskDescriptor* disk = GetDiskDescriptor(disk_id);
    Lba48 base = GetPartitionStart(disk, partition);
    return disk->read_op(disk, base + sector, buf);
}
