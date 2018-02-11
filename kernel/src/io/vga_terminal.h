// File:       io/vga_terminal.h
// Author:     vodozhaba
// Created on: Aug 20, 2016
// Purpose:    Provides an interface for VGA 80x25 terminal.

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
} __attribute__((packed)) VgaColorScheme;

typedef struct {
    uint8_t character;
    VgaColorScheme color_scheme;
} __attribute__((packed)) VgaEntry;

static const VgaColorScheme default_color_scheme = {
        .foreground = VGA_COLOR_LIGHT(VGA_COLOR_CYAN),
        .background = VGA_COLOR_BLACK
};
static const VgaColorScheme err_color_scheme = {
        .foreground = VGA_COLOR_LIGHT(VGA_COLOR_RED),
        .background = VGA_COLOR_BLACK
};
static const VgaColorScheme link_color_scheme = {
        .foreground = VGA_COLOR_LIGHT(VGA_COLOR_BLUE),
        .background = VGA_COLOR_BLACK
};

void VgaTerminalInit();
void VgaTerminalPut(char c);
VgaColorScheme VgaTerminalGetColorScheme();
void VgaTerminalSwitchColorScheme(VgaColorScheme color_scheme);
