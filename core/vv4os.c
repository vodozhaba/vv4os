/******************************************************************************
 * File:       core/vv4os.c                                                   *
 * Author:     velikiyv4                                                      *
 * Created on: Aug 20, 2016                                                   *
 * Purpose:    The main file of the project, connects all the modules.        *
 ******************************************************************************/

#include "io/vga_terminal.h"

void main() {
    VgaTerminalInit();
    VgaTerminalPut('A');
    vga_color_scheme red = {
            .foreground = VGA_COLOR_RED,
            .background = VGA_COLOR_BLACK
    };
    VgaTerminalSwitchColorScheme(red);
}
