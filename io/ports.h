/******************************************************************************
 * File:       io/ports.h                                                     *
 * Author:     velikiyv4                                                      *
 * Created on: 08/21/2016                                                     *
 * Purpose:    Provides functions for port I/O.                               *
 ******************************************************************************/

#pragma once

#include "stdint.h"

inline void PortWrite8(uint16_t port, uint8_t value) {
    __asm volatile("outb %b0, %w1" : : "a" (value), "d" (port));
}

inline uint8_t PortRead8(uint16_t port) {
    uint8_t ret;
    __asm volatile("inb %w1, %b0" : "=a" (ret) : "d" (port));
    return ret;
}
