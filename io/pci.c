/******************************************************************************
 * File:       /vv4os/io/pci.c                                                *
 * Author:     vodozhaba                                                      *
 * Created on: Apr 23, 2017                                                   *
 * Purpose:    Allows working with the PCI bus.                               *
 ******************************************************************************/

#include "pci.h"
#include <stdbool.h>
#include <stdint.h>
#include "io/ports.h"

uint16_t PciConfigReadWord(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
	PciConfigAddress addr;
	addr.as_struct.zero = 0;
	addr.as_struct.reg = offset >> 2;
	addr.as_struct.function = function;
	addr.as_struct.device = device;
	addr.as_struct.bus = bus;
	addr.as_struct.reserved = 0;
	addr.as_struct.enable = true;
	PortWrite32(PCI_CONFIG_ADDRESS_PORT, addr.as_u32);
	if(offset % 32 == 0)
		return (uint16_t) PortRead32(PCI_CONFIG_DATA_PORT);
	else
		return (uint16_t)(PortRead32(PCI_CONFIG_DATA_PORT) >> 16);
}
