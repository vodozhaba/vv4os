/******************************************************************************
 * File:       io/vga_terminal.h                                              *
 * Author:     velikiyv4                                                      *
 * Created on: 08/20/2016                                                     *
 * Purpose:    Provides an interface for VGA 80x25 terminal.                  *
 ******************************************************************************/

#pragma once

#include <stdint.h>

#define VGA_COLOR_BLACK    0x0
#define VGA_COLOR_BLUE     0x1
#define VGA_COLOR_GREEN    0x2
#define VGA_COLOR_CYAN     0x3
#define VGA_COLOR_RED      0x4
#define VGA_COLOR_MAGENTA  0x5
#define VGA_COLOR_BROWN    0x6
#define VGA_COLOR_GRAY     0x7

#define VGA_COLOR_LIGHT(color) ((color) | 0x8)

typedef struct {
    uint8_t foreground :4;
    uint8_t background :4;
} __attribute__((packed)) vga_color_scheme;

typedef struct {
    uint8_t character;
    vga_color_scheme color_scheme;
} __attribute__((packed)) vga_entry;

int VgaTerminalInit();
