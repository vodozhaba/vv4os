/******************************************************************************
 * File:       /vv4os/io/pci.h                                                *
 * Author:     vodozhaba                                                      *
 * Created on: Apr 22, 2017                                                   *
 * Purpose:    Allows working with the PCI bus.                               *
 ******************************************************************************/

#pragma once

#include <stdint.h>

typedef struct PciDevice {
	uint16_t vendor_id;
	uint16_t device_id;
	uint16_t command;
	uint16_t status;
	uint8_t revision_id;
	uint8_t prog_if;
	uint8_t subclass;
	uint8_t class_code;
	uint8_t cache_line_size;
	uint8_t latency_timer;
	uint8_t header_type;
	uint8_t bist;
	uint32_t bar0;
	uint32_t bar1;
	uint32_t bar2;
	uint32_t bar3;
	uint32_t bar4;
	uint32_t bar5;
	uint32_t cardbus_cis_pointer;
	uint16_t subsystem_vendor_id;
	uint16_t sybsystem_id;
	uint32_t expansion_rom_base_address;
	uint8_t capabilities_pointer;
	uint64_t reserved : 56;
	uint8_t interrupt_line;
	uint8_t interrupt_pin;
	uint8_t min_grant;
	uint8_t max_latency;
	uint8_t bus;
	uint8_t device : 5;
	uint8_t function : 3;
	struct PciDevice* next;
} __attribute__((packed)) PciDevice;

extern PciDevice* pci_devices_list;

void PciInit();
