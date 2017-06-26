/******************************************************************************
 * File:       /vv4os/core/config.h                                           *
 * Author:     vodozhaba                                                      *
 * Created on: Jun 19, 2017                                                   *
 * Purpose:    Allows configuring VV4OS.                                      *
 ******************************************************************************/

#pragma once

/* Debugging */

// If defined, starts a GDB stub at the first COM port in the debugging version.
#define UART_DEBUGGING

// If defined, sets a breakpoint as soon as possible and waits for GDB connection.
#define WAIT_FOR_GDB

// Defines the maximum size for a GDB packet.

#define GDB_PACKET_BUF_SIZE 400

// If defined, defines the maximum amount of send attempts by the GDB stub.
// Otherwise the amount of attempts is unlimited.
#define GDB_MAX_SEND_ATTEMPTS 3
