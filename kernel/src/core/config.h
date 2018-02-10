// File:       core/config.h
// Author:     vodozhaba
// Created on: Jun 19, 2017
// Purpose:    Allows configuring VV4OS.

#pragma once

/* Disk subsystem */

// The max amount of disks that can be attached to the system.
#define MAX_DISKS 4096

// If defined, the first disk detected is assigned the index 0, otherwise 1
// #define DISK_INDICES_START_FROM_ZERO


/* Debugging */

// If defined, starts a GDB stub at the first COM port in the debugging version.
#define UART_DEBUGGING

// If defined, sets a breakpoint as soon as possible and waits for GDB connection.
// #define WAIT_FOR_GDB

// Defines the maximum size for a GDB packet.

#define GDB_PACKET_BUF_SIZE 400

// If defined, defines the maximum amount of send attempts by the GDB stub.
// Otherwise the amount of attempts is unlimited.
#define GDB_MAX_SEND_ATTEMPTS 3


/* User space */

// Defines the base address user processes are loaded at.
#define USER_PROCESS_BASE ((void*) 0x00100000)

// Defines the interrupt which can be called by user-space code to interact with the kernel
#define USER_SYSCALL_INTERRUPT 0x80