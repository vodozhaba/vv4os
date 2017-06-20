/******************************************************************************
 * File:       /vv4os/core/config.h                                           *
 * Author:     vodozhaba                                                      *
 * Created on: Jun 19, 2017                                                   *
 * Purpose:    Allows configuring VV4OS.                                      *
 ******************************************************************************/

#pragma once

// If defined, starts a GDB stub at the first COM port in the debugging version.
#define UART_DEBUGGING

// If defined, sets a breakpoint as soon as possible and waits for GDB connection
#define WAIT_FOR_GDB
