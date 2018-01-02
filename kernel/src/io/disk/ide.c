// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       io/disk/ide.c
// Author:     vodozhaba
// Created on: Jan 02, 2017
// Purpose:    Works with PCI IDE controllers.                                   

#include "io/disk/ide.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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
        uint16_t status;
        while((status = PortRead8(channel + 7)) & 0x08 & 0x01);
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

void IdeControllerInit(PciDevice* controller) {
    // Only configure one IDE controller.
    // TODO: add support for multiple controllers with port remapping and stuff
    static bool one = false;
    if(one) {
        controller->data = NULL;
        return;
    }
    one = true;
    IdeControllerInternalData* data = malloc(sizeof(*data));
    data->p_channel = 0x1F0;
    data->p_control = 0x3F6;
    data->s_channel = 0x170;
    data->s_control = 0x376;
    IdentifyDrives(data);
    controller->data = data;
}
