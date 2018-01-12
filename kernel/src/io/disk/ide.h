// File:       io/disk/ide.h
// Author:     vodozhaba
// Created on: Jan 02, 2017
// Purpose:    Works with PCI IDE controllers.

#pragma once

#include "io/disk/disk.h"
#include "io/pci.h"

DiskDescriptor* IdeControllerInit(PciDevice* controller);
