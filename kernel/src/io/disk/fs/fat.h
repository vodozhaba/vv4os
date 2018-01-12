// File:       io/disk/fs/fat.c
// Author:     vodozhaba
// Created on: Jan 03, 2017
// Purpose:    Provides file functions for FAT12/16/32 partitions.                                   

#pragma once

#include <stdbool.h>
#include "io/disk/disk.h"
#include "io/disk/volume.h"

Volume* FatMountVolume(DiskDescriptor* disk, uint32_t partition);
FileDescriptor* Fat32TraverseOp(FileDescriptor* parent, char* name);
size_t Fat32ReadOp(FileDescriptor* file, size_t size, void* buf);