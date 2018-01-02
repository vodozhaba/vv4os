// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       io/disk/ide.c
// Author:     vodozhaba
// Created on: Jan 02, 2017
// Purpose:    Works with PCI IDE controllers.                                   

#include "io/disk/ide.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "io/disk/disk.h"
#include "io/ports.h"

typedef struct {
    // Ports
    uint16_t p_channel;
    uint16_t p_control;
    uint16_t s_channel;
    uint16_t s_control;
    // Drives existence
    bool pm : 1;
    bool ps : 1;
    bool sm : 1;
    bool ss : 1;
} IdeControllerInternalData;

typedef struct {
    IdeControllerInternalData* icid;
    bool secondary : 1;
    bool slave : 1;
} IdeDiskInternalData;

static bool IdentifyDrive(uint16_t channel, bool slave) {
    uint8_t cmd = slave ? 0xB0 : 0xA0;
    if(PortRead8(channel + 7) == 0xFF) {
        return false;
    } else {
        PortWrite8(channel + 6, cmd);
        PortWrite8(channel + 2, 0x00);
        PortWrite8(channel + 3, 0x00);
        PortWrite8(channel + 4, 0x00);
        PortWrite8(channel + 5, 0x00);
        PortWrite8(channel + 7, 0xEC);
        if(PortRead8(channel + 7) == 0) {
            return false;
        }
        while(PortRead8(channel + 7) & 0x80);
        if(PortRead8(channel + 4) || PortRead8(channel + 5)) {
            return false;
        }
        uint8_t status;
        while(true) {
            status = PortRead8(channel + 7);
            if(!(status & 0x08) || !(status & 0x01)) {
                break;
            }
        }
        if(!(status & 0x01)) {
            for(size_t i = 0; i < 256; i++) {
                PortRead16(channel);
            }
        }
        return true;
    }
}

static void IdentifyDrives(IdeControllerInternalData* data) {
    data->pm = IdentifyDrive(data->p_channel, false);
    data->ps = IdentifyDrive(data->p_channel, true);
    data->sm = IdentifyDrive(data->s_channel, false);
    data->ss = IdentifyDrive(data->s_channel, true);
}

void* IdeReadOp(DiskDescriptor* disk, Lba48 sector, void* buf) {
    IdeDiskInternalData* disk_data = disk->data;
    assert(disk_data);
    IdeControllerInternalData* controller_data = disk_data->icid;
    assert(controller_data);
    uint16_t channel = disk_data->secondary ? controller_data->s_channel : controller_data->p_channel;
    uint8_t cmd = disk_data->slave ? 0x50 : 0x40;
    PortWrite8(channel + 6, cmd);
    PortWrite8(channel + 2, 0x00);
    PortWrite8(channel + 3, sector >> 24);
    PortWrite8(channel + 4, sector >> 32);
    PortWrite8(channel + 5, sector >> 40);
    PortWrite8(channel + 2, 0x01);
    PortWrite8(channel + 3, sector);
    PortWrite8(channel + 4, sector >> 8);
    PortWrite8(channel + 5, sector >> 16);
    PortWrite8(channel + 7, 0x24);
    for(size_t i = 0; i < 4; i++) {
        PortRead8(channel + 7); // 400ns delay
    }
    while(true) {
        uint8_t status = PortRead8(channel + 7);
        if(!(status & 0x80) && status & 0x08) {
            break;
        }
    }
    for(size_t i = 0; i < 256; i++) {
        ((uint16_t*) buf)[i] = PortRead16(channel);
    }
    return buf;
}

DiskDescriptor* IdeControllerInit(PciDevice* controller) {
    // Only configure one IDE controller.
    // TODO: add support for multiple controllers with port remapping and stuff
    static bool one = false;
    if(one) {
        controller->data = NULL;
        return NULL;
    }
    one = true;
    IdeControllerInternalData* controller_data = malloc(sizeof(*controller_data));
    controller_data->p_channel = 0x1F0;
    controller_data->p_control = 0x3F6;
    controller_data->s_channel = 0x170;
    controller_data->s_control = 0x376;
    IdentifyDrives(controller_data);
    DiskDescriptor* next = NULL;
    for(size_t i = 0; i < 4; i++) {
        bool exists;
        switch(i) {
            case 0:
                exists = controller_data->pm;
                break;
            case 1:
                exists = controller_data->ps;
                break;
            case 2:
                exists = controller_data->sm;
                break;
            case 3:
                exists = controller_data->ss;
                break;
        }
        if(!exists) {
            continue;
        }
        bool secondary = i / 2;
        bool slave = i % 2;
        IdeDiskInternalData* disk_data = malloc(sizeof(*disk_data));
        disk_data->icid = controller_data;
        disk_data->secondary = secondary;
        disk_data->slave = slave;
        DiskDescriptor* disk = malloc(sizeof(*disk));
        disk->data = disk_data;
        disk->next = next;
        disk->read_op = IdeReadOp;
        next = disk;
    }
    controller->data = controller_data;
    return next;
}
