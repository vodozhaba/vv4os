/******************************************************************************
 * File:       /vv4os/io/vga_terminal.c                                              *
 * Author:     velikiyv4                                                      *
 * Created on: Aug 20, 2016                                                 *
 * Purpose:    Module for VGA 80x25 terminal.                                 *
 ******************************************************************************/

#include "vga_terminal.h"
#include <stddef.h>
#include "ports.h"

static const int VGA_BUFFER_HEIGHT = 25, VGA_BUFFER_WIDTH = 80;
static const uint8_t STD_FG_COLOR = VGA_COLOR_LIGHT(VGA_COLOR_CYAN),
        STD_BG_COLOR = VGA_COLOR_BLACK;

static vga_color_scheme current_color_scheme;
static vga_entry* vga_buffer = (vga_entry*) 0xB8000;
static int current_x = 0, current_y = 0;

static int SetEntryAt(int x, int y, vga_entry entry) {
    if (x < 0 || x >= VGA_BUFFER_WIDTH || y < 0 || y >= VGA_BUFFER_HEIGHT)
        return -1;
    int i = y * VGA_BUFFER_WIDTH + x;
    vga_buffer[i] = entry;
    return 0;
}

static int GetEntryAt(int x, int y, vga_entry* entry) {
    if (x < 0 || x >= VGA_BUFFER_WIDTH || y < 0 || y >= VGA_BUFFER_HEIGHT ||
            entry == NULL)
        return -1;
    int i = y * VGA_BUFFER_WIDTH + x;
    *entry = vga_buffer[i];
    return 0;
}

static vga_entry ConstructVgaEntry (uint8_t character,
        vga_color_scheme color_scheme) {
    vga_entry ret;
    ret.character = character;
    ret.color_scheme = color_scheme;
    return ret;
}

static int MoveCursorAt(int x, int y) {
    if (x < 0 || x >= VGA_BUFFER_WIDTH || y < 0 || y >= VGA_BUFFER_HEIGHT)
        return -1;
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

static int Scroll() {
    int res;
    vga_entry entry;
    for(int y = 1; y < VGA_BUFFER_HEIGHT; ++y) {
        for(int x = 0; x < VGA_BUFFER_WIDTH; ++x) {
            res = GetEntryAt(x, y, &entry);
            if(res != 0)
                return res;
            res = SetEntryAt(x, y - 1, entry);
            if(res != 0)
                return res;
        }
    }
    return MoveCursorAt(current_x, current_y - 1);
}

static int AdvanceCursor() {
    int res;
    if(current_x == VGA_BUFFER_WIDTH - 1) {
        res = MoveCursorAt(0, current_y);
        if(res != 0)
            return res;
        if(current_y == VGA_BUFFER_HEIGHT - 1) {
            res = Scroll();
            if(res != 0)
                return res;
        }
        return MoveCursorAt(current_x, current_y + 1);
    }
    return MoveCursorAt(current_x + 1, current_y);
}

int VgaTerminalInit() {
    MoveCursorAt(0, 0);
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

int VgaTerminalPut(char c) {
    int res;
    res = SetEntryAt(current_x, current_y,
            ConstructVgaEntry(c, current_color_scheme));
    if(res != 0)
        return res;
    return AdvanceCursor();
}

void VgaTerminalSwitchColorScheme(vga_color_scheme color_scheme) {
    current_color_scheme = color_scheme;
}
