// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       io/vga_terminal.c
// Author:     vodozhaba                                                      
// Created on: Aug 20, 2016                                                   
// Purpose:    Module for VGA 80x25 terminal.                                 

#include "vga_terminal.h"
#include <stddef.h>
#include <assert.h>
#include "ports.h"

static const unsigned int VGA_BUFFER_HEIGHT = 25, VGA_BUFFER_WIDTH = 80;
static const uint8_t STD_FG_COLOR = VGA_COLOR_LIGHT(VGA_COLOR_CYAN),
        STD_BG_COLOR = VGA_COLOR_BLACK;

static VgaColorScheme current_color_scheme;
static volatile VgaEntry* vga_buffer = (VgaEntry*) 0xC00B8000;
static unsigned int current_x = 0, current_y = 0;

static void SetEntryAt(unsigned int x, unsigned int y, VgaEntry entry) {
    assert (x < VGA_BUFFER_WIDTH && y < VGA_BUFFER_HEIGHT);
    int i = y * VGA_BUFFER_WIDTH + x;
    vga_buffer[i] = entry;
}

static VgaEntry GetEntryAt(unsigned int x, unsigned int y) {
    assert (x < VGA_BUFFER_WIDTH && y < VGA_BUFFER_HEIGHT);
    int i = y * VGA_BUFFER_WIDTH + x;
    return vga_buffer[i];
}

static VgaEntry ConstructVgaEntry (uint8_t character,
        VgaColorScheme color_scheme) {
    VgaEntry ret;
    ret.character = character;
    ret.color_scheme = color_scheme;
    return ret;
}

static int MoveCursorAt(unsigned int x, unsigned int y) {
    assert (x < VGA_BUFFER_WIDTH && y < VGA_BUFFER_HEIGHT);
    current_x = x;
    current_y = y;
    /* Moving hardware cursor */
    int pos = y * VGA_BUFFER_WIDTH + x;
    uint16_t low = (uint16_t)(pos & 0xFF) << 8 | 0x0F;
    uint16_t high = (uint16_t)(pos & 0xFF00) | 0x0E;
    PortWrite16(0x3D4, low);
    PortWrite16(0x3D4, high);
    return 0;
}

static void Scroll() {
    for(unsigned int y = 1; y < VGA_BUFFER_HEIGHT; ++y) {
        for(unsigned int x = 0; x < VGA_BUFFER_WIDTH; ++x) {
            VgaEntry entry = GetEntryAt(x, y);
            SetEntryAt(x, y - 1, entry);
        }
    }
    VgaEntry whitespace = ConstructVgaEntry(' ', current_color_scheme);
    for(unsigned int x = 0; x < VGA_BUFFER_WIDTH; ++x) {
        SetEntryAt(x, VGA_BUFFER_HEIGHT - 1, whitespace);
    }
    MoveCursorAt(current_x, current_y - 1);
}

static void AdvanceCursor(int n) {
    unsigned int value = current_x + n; /* I don't know how to name it */
    unsigned int lines = value / VGA_BUFFER_WIDTH;
    unsigned int new_x = value % VGA_BUFFER_WIDTH;
    while(current_y + lines >= VGA_BUFFER_HEIGHT) {
        Scroll();
    }
    MoveCursorAt(new_x, current_y + lines);
}

void VgaTerminalInit() {
    MoveCursorAt(0, 0);
    current_color_scheme.foreground = STD_FG_COLOR;
    current_color_scheme.background = STD_BG_COLOR;
    VgaEntry whitespace = ConstructVgaEntry(' ', current_color_scheme);
    for (unsigned int y = 0; y < VGA_BUFFER_HEIGHT; y++) {
        for (unsigned int x = 0; x < VGA_BUFFER_WIDTH; x++) {
            SetEntryAt(x, y, whitespace);
        }
    }
}

void VgaTerminalPut(char c) {
    switch(c) {
        case '\n':
            return AdvanceCursor(VGA_BUFFER_WIDTH - current_x);
        default:
            SetEntryAt(current_x, current_y,
                    ConstructVgaEntry(c, current_color_scheme));
            AdvanceCursor(1);
    }
}

VgaColorScheme VgaTerminalGetColorScheme() {
    return current_color_scheme;
}

void VgaTerminalSwitchColorScheme(VgaColorScheme color_scheme) {
    current_color_scheme = color_scheme;
}
