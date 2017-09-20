/******************************************************************************
 * File:       /vv4os/io/pci.c                                                *
 * Author:     vodozhaba                                                      *
 * Created on: Apr 23, 2017                                                   *
 * Purpose:    Allows working with the PCI bus.                               *
 ******************************************************************************/

#include "pci.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "arch/x86/dt/idt_x86.h"
#include "io/ports.h"
#include "stdlib/stdlib.h"

#define PCI_CONFIG_ADDRESS_PORT 0xCF8
#define PCI_CONFIG_DATA_PORT 0xCFC

#define PCI_CONFIG_CLASS_CODE_BRIDGE 0x06

#define PCI_CONFIG_SUBCLASS_PCI_TO_PCI_BRIDGE 0x04

#define PCI_IRQ 10

typedef union {
	struct {
		uint8_t zero : 2;
		uint8_t reg : 6;
		uint8_t function : 3;
		uint8_t device : 5;
		uint8_t bus;
		uint8_t reserved : 7;
		bool enable : 1;
	} __attribute__((packed)) as_struct;
	uint32_t as_u32;
} PciConfigAddress;

PciDevice* pci_devices_list = NULL;

static uint32_t PciConfigRead32(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
	PciConfigAddress addr;
	addr.as_struct.zero = 0;
	addr.as_struct.reg = offset >> 2;
	addr.as_struct.function = function;
	addr.as_struct.device = device;
	addr.as_struct.bus = bus;
	addr.as_struct.reserved = 0;
	addr.as_struct.enable = true;
	PortWrite32(PCI_CONFIG_ADDRESS_PORT, addr.as_u32);
	return PortRead32(PCI_CONFIG_DATA_PORT);
}

static uint8_t PciConfigRead8(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
	return (uint8_t)(PciConfigRead32(bus, device, function, offset) >> (offset % 4) * 8);
}

static uint16_t PciConfigRead16(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
	return (uint16_t)(PciConfigRead32(bus, device, function, offset) >> (offset % 4) * 8);
}

static void PciConfigWrite32(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset, uint32_t data) {
	PciConfigAddress addr;
	addr.as_struct.zero = 0;
	addr.as_struct.reg = offset >> 2;
	addr.as_struct.function = function;
	addr.as_struct.device = device;
	addr.as_struct.bus = bus;
	addr.as_struct.reserved = 0;
	addr.as_struct.enable = true;
	PortWrite32(PCI_CONFIG_ADDRESS_PORT, addr.as_u32);
	PortWrite32(PCI_CONFIG_DATA_PORT, data);
}

static void PciConfigWrite8(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset, uint8_t data) {
	uint32_t reg = PciConfigRead32(bus, device, function, offset);
	reg &= ~(0xFF << (offset % 4));
	reg |= data << (offset % 4);
	PciConfigWrite32(bus, device, function, offset, reg);
}

static void PciConfigWrite16(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset, uint16_t data) {
	uint32_t reg = PciConfigRead32(bus, device, function, offset);
	reg &= ~(0xFFFF << (offset % 4));
	reg |= data << (offset % 4);
	PciConfigWrite32(bus, device, function, offset, reg);
}

static PciConfigHeaderType PciConfigReadHeaderType(uint8_t bus, uint8_t device, uint8_t function) {
	PciConfigHeaderType ret;
	uint8_t byte = PciConfigRead8(bus, device, function, PCI_CONFIG_COMMON_OFFSET_HEADER_TYPE);
	ret.mf = byte >> 7;
	ret.header_type = byte;
	return ret;
}

static PciConfigBistRegister PciConfigReadBistRegister(uint8_t bus, uint8_t device, uint8_t function) {
	PciConfigBistRegister ret;
	uint8_t byte = PciConfigRead8(bus, device, function, PCI_CONFIG_COMMON_OFFSET_BIST_REGISTER);
	ret.completion_code = byte;
	ret.reserved = 0;
	ret.start_bist = byte >> 6;
	ret.bist_capable = byte >> 7;
	return ret;
}

static void PciProbeBus(uint8_t bus);

static void PciAddDevice(uint8_t bus, uint8_t device, uint8_t function) {
	PciConfigWrite8(bus, device, function, PCI_CONFIG_REGULAR_DEVICE_OFFSET_INTERRUPT_LINE, PCI_IRQ);
	PciDevice* pci_device = malloc(sizeof(*pci_device));
	pci_device->bus = bus;
	pci_device->device = device;
	pci_device->function = function;
	pci_device->next = pci_devices_list;
	pci_devices_list = pci_device;
}

static void PciProbeFunction(uint8_t bus, uint8_t device, uint8_t function) {
	uint8_t class = PciConfigRead8(bus, device, function, PCI_CONFIG_COMMON_OFFSET_CLASS_CODE);
	uint8_t subclass = PciConfigRead8(bus, device, function, PCI_CONFIG_COMMON_OFFSET_SUBCLASS);
	if(class == PCI_CONFIG_CLASS_CODE_BRIDGE && subclass == PCI_CONFIG_SUBCLASS_PCI_TO_PCI_BRIDGE) {
		uint8_t secondary_bus = PciConfigRead8(bus, device, function, PCI_CONFIG_PCI_TO_PCI_BRIDGE_OFFSET_SECONDARY_BUS_NUMBER);
		PciProbeBus(secondary_bus);
	}
	PciAddDevice(bus, device, function);
}

static void PciProbeDevice(uint8_t bus, uint8_t device) {
	if(PciConfigRead16(bus, device, 0, PCI_CONFIG_COMMON_OFFSET_VENDOR_ID) == 0xFFFF)
		return;
	PciProbeFunction(bus, device, 0);
	PciConfigHeaderType ht = PciConfigReadHeaderType(bus, device, 0);
	if(!ht.mf)
		return;

	for(uint8_t function = 1; function < 8; function++) {
		if(PciConfigRead16(bus, device, function, PCI_CONFIG_COMMON_OFFSET_VENDOR_ID) != 0xFFFF)
			PciProbeFunction(bus, device, function);
	}
}

static void PciProbeBus(uint8_t bus) {
	for(uint8_t device = 0; device < 32; device++) {
		PciProbeDevice(bus, device);
	}
}

static void PciIrqHandler() {

}

void PciInit() {
	PciConfigHeaderType ht = PciConfigReadHeaderType(0, 0, 0);
	PciProbeBus(0);
	if(ht.mf) {
		for(uint8_t function = 0; function < 8; function++) {
			uint16_t vendor_id = PciConfigRead16(0, 0, function, PCI_CONFIG_COMMON_OFFSET_VENDOR_ID);
			if(vendor_id == 0xFFFF)
				break;
			PciProbeBus(function);
		}
	}
#if defined(__X86__)
	X86RegisterIrqHandler(IRQ(PCI_IRQ), PciIrqHandler);
#endif
}

uint8_t PciConfigReadFromDevice8(PciDevice* device, uint8_t offset) {
	return PciConfigRead8(device->bus, device->device, device->function, offset);
}
uint16_t PciConfigReadFromDevice16(PciDevice* device, uint8_t offset) {
	return PciConfigRead16(device->bus, device->device, device->function, offset);
}
uint32_t PciConfigReadFromDevice32(PciDevice* device, uint8_t offset) {
	return PciConfigRead32(device->bus, device->device, device->function, offset);
}

void PciConfigWriteToDevice8(PciDevice* device, uint8_t offset, uint8_t data) {
	return PciConfigWrite8(device->bus, device->device, device->function, offset, data);
}
void PciConfigWriteToDevice16(PciDevice* device, uint8_t offset, uint16_t data) {
	return PciConfigWrite16(device->bus, device->device, device->function, offset, data);
}
void PciConfigWriteToDevice32(PciDevice* device, uint8_t offset, uint32_t data) {
	return PciConfigWrite32(device->bus, device->device, device->function, offset, data);
}

PciConfigHeaderType PciConfigReadHeaderTypeFromDevice(PciDevice* device) {
	return PciConfigReadHeaderType(device->bus, device->device, device->function);
}

PciConfigBistRegister PciConfigReadBistRegisterFromDevice(PciDevice* device) {
	return PciConfigReadBistRegister(device->bus, device->device, device->function);
}



