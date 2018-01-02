// File:       io/disk/ide.h
// Author:     vodozhaba
// Created on: Jan 02, 2017
// Purpose:    Works with PCI IDE controllers.

#pragma once

#include "io/pci.h"

void IdeControllerInit(PciDevice* controller);
