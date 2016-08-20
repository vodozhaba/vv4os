/******************************************************************************
 * File:       io/vga_terminal.c                                              *
 * Author:     velikiyv4                                                      *
 * Created on: 08/20/2016                                                     *
 * Purpose:    Module for VGA 80x25 terminal.                                 *
 ******************************************************************************/

#include "vga_terminal.h"

static const int VGA_BUFFER_HEIGHT = 25, VGA_BUFFER_WIDTH = 80;
static const uint8_t STD_FG_COLOR = VGA_COLOR_LIGHT(VGA_COLOR_CYAN),
        STD_BG_COLOR = VGA_COLOR_BLACK;

static vga_color_scheme current_color_scheme;
static vga_entry* vga_buffer = (vga_entry*) 0xB8000;

static int SetEntryAt(int x, int y, vga_entry entry) {
    if (x < 0 || x >= VGA_BUFFER_WIDTH || y < 0 || y >= VGA_BUFFER_HEIGHT)
        return -1;
    int i = y * VGA_BUFFER_WIDTH + x;
    vga_buffer[i] = entry;
    return 0;
}

static vga_entry ConstructVgaEntry (uint8_t character,
        vga_color_scheme color_scheme) {
    vga_entry ret;
    ret.character = character;
    ret.color_scheme = color_scheme;
    return ret;
}

int VgaTerminalInit() {
    current_color_scheme.foreground = STD_FG_COLOR;
    current_color_scheme.background = STD_BG_COLOR;
    vga_entry whitespace = ConstructVgaEntry(' ', current_color_scheme);
    for (int y = 0; y < VGA_BUFFER_HEIGHT; y++) {
        for (int x = 0; x < VGA_BUFFER_WIDTH; x++) {
            int ret = SetEntryAt(x, y, whitespace);
            if (ret != 0)
                return ret;
        }
    }
    return 0;
}
