// File:       io/disk/volume.h
// Author:     vodozhaba
// Created on: Jan 03, 2017
// Purpose:    Abstraction layer over file systems.

#pragma once

#include "io/disk/disk.h"
#include "io/disk/file.h"

typedef struct Volume {
    DiskDescriptor* disk;
    void* data;
    uint32_t partition;
    FileDescriptor* root;
    struct Volume* next;
} Volume;

void AddVolumes(Volume* chain);
Volume* GetVolume(DiskDescriptor* disk, uint32_t partition);