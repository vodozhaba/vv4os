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

inline void PortWrite16(uint16_t port, uint16_t value) {
    __asm volatile("outw %w0, %w1" : : "a" (value), "d" (port));
}

inline uint16_t PortRead16(uint16_t port) {
    uint16_t ret;
    __asm volatile("inw %w1, %w0" : "=a" (ret) : "d" (port));
    return ret;
}

inline void PortWrite32(uint16_t port, uint32_t value) {
    __asm volatile("outl %w0, %w1" : : "a" (value), "d" (port));
}

inline uint32_t PortRead32(uint16_t port) {
    uint32_t ret;
    __asm volatile("inl %w1, %w0" : "=a" (ret) : "d" (port));
    return ret;
}
