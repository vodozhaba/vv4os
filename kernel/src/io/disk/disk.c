// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       io/disk/disk.c
// Author:     vodozhaba
// Created on: Apr 24, 2017
// Purpose:    Controls the disk subsystem.                                   

#include <stddef.h>
#include <stdio.h>
#include "io/pci.h"

#define PCI_SUBCLASS_SCSI_BUS_CONTROLLER 0x00
#define PCI_SUBCLASS_IDE_CONTROLLER 0x01
#define PCI_SUBCLASS_FLOPPY_DISK_CONTROLLER 0x02
#define PCI_SUBCLASS_IPI_BUS_CONTROLLER 0x03
#define PCI_SUBCLASS_RAID_CONTROLLER 0x04
#define PCI_SUBCLASS_ATA_CONTROLLER 0x05
#define PCI_SUBCLASS_SATA_CONTROLLER 0x06
#define PCI_SUBCLASS_SAS_CONTROLLER 0x07
#define PCI_SUBCLASS_OTHER_MASS_STORAGE_CONTROLLER 0x80

void DiskSubsystemInit() {
    for(PciDevice* controller = pci_devices_list; controller != NULL; controller = controller->next) {
	    uint8_t class_code = PciConfigReadFromDevice8(controller, PCI_CONFIG_COMMON_OFFSET_CLASS_CODE);
	    if(class_code == PCI_CLASS_CODE_MASS_STORAGE_CONTROLLER) {
    	    uint8_t subclass = PciConfigReadFromDevice8(controller, PCI_CONFIG_COMMON_OFFSET_SUBCLASS);
    	    switch(subclass) {
                default:
                    printf("Detected an unsupported mass storage controller.\nSome drives might be absent.\n");
    	    }
	    }
    }
}
