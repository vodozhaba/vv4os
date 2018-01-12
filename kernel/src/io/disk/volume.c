// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       io/disk/volume.c
// Author:     vodozhaba
// Created on: Jan 03, 2017
// Purpose:    Abstraction layer over file systems.

#include "io/disk/volume.h"
#include <stddef.h>
#include <stdint.h>

static Volume* volumes = NULL;

void AddVolumes(Volume* chain) {
    if(volumes == NULL) {
        volumes = chain;
        return;
    }
    Volume* head;
    for(head = volumes; head; head = head->next);
    head->next = chain;
}

Volume* GetVolume(DiskDescriptor* disk, uint32_t partition) {
    for(Volume* volume = volumes; volume; volume = volume->next) {
        if(volume->disk == disk && volume->partition == partition) {
            return volume;
        }
    }
    return NULL;
}