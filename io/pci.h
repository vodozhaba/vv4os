/******************************************************************************
 * File:       /vv4os/io/pci.h                                                *
 * Author:     vodozhaba                                                      *
 * Created on: Apr 22, 2017                                                   *
 * Purpose:    Allows working with the PCI bus.                               *
 ******************************************************************************/

#define PCI_CONFIG_ADDRESS_PORT 0xCF8
#define PCI_CONFIG_DATA_PORT 0xCFC

#include <stdbool.h>
#include <stdint.h>

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

uint16_t PciConfigReadWord(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset);
