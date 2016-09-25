/******************************************************************************
 * File:       /vv4os/stdlib/stdio.c                                          *
 * Author:     velikiyv4                                                      *
 * Created on: Aug 21, 2016                                                   *
 * Purpose:    Provides standard I/O functions.                               *
 ******************************************************************************/

#include "stdio.h"
#include "stdlib.h"
#include "io/vga_terminal.h"
#include <stdarg.h>
#include <stddef.h>

int putchar(int character) {
    return VgaTerminalPut((char) character);
}

int _puts(const char* s) {
    for(size_t i = 0; s[i] != 0x00; ++i) {
        int res = putchar(s[i]);
        if(res != 0)
            return res;
    }
    return 0;
}

int printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    for(size_t i = 0; fmt[i]; ++i) {
        if(fmt[i] != '%') {
            VgaTerminalPut(fmt[i]);
            continue;
        }
        char c = fmt[++i];
        switch(c) {
            case '%': {
                putchar('%');
                break;
            }
            case 'd':
            case 'i': {
                int arg = va_arg(args, int);
                char str[32];
                itoa(arg, str, 10);
                puts(str);
                break;
            }
            case 'u': {
                unsigned int arg = va_arg(args, unsigned int);
                char str[32];
                utoa(arg, str, 10);
                puts(str);
                break;
            }
            case 'o': {
                unsigned int arg = va_arg(args, unsigned int);
                char str[32];
                utoa(arg, str, 8);
                puts(str);
                break;
            }
            case 'x': {
                unsigned int arg = va_arg(args, unsigned int);
                char str[32];
                utoa_lc(arg, str, 16);
                puts(str);
                break;
            }
            case 'X': {
                unsigned int arg = va_arg(args, unsigned int);
                char str[32];
                utoa(arg, str, 16);
                puts(str);
                break;
            }
            case 'c': {
                char arg = va_arg(args, int);
                putchar(arg);
                break;
            }
            case 's': {
                char* arg = va_arg(args, char*);
                puts(arg);
                break;
            }
        }
    }
    va_end(args);
    /* TODO add valid return value */
    return 0;
}
