// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       stdlib/stdlib.c
// Author:     vodozhaba
// Created on: Aug 21, 2016
// Purpose:    Provides standard functions.

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "io/vga_terminal.h"

static unsigned int digits(unsigned int x, int base) {
    if(x == 0)
        return 1;
    unsigned int ret;
    for(ret = 0; x > 0; x /= base, ++ret);
    return ret;
}

char* utoa(unsigned int value, char* str, int base) {
    if(base < 2 || base > 36)
        return NULL;
    static const char* alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int len = digits(value, base);
    for(int i = len - 1; i >= 0; --i) {
        str[i] = alphabet[value % base];
        value /= base;
    }
    str[len] = 0;
    return str;
}

char* utoa_lc(unsigned int value, char* str, int base) {
    if(base < 2 || base > 36)
        return NULL;
    static const char* alphabet = "0123456789abcdefghijklmnopqrstuvwxyz";
    int len = digits(value, base);
    for(int i = len - 1; i >= 0; --i) {
        str[i] = alphabet[value % base];
        value /= base;
    }
    str[len] = 0;
    return str;
}

char* itoa(int value, char* str, int base) {
    if(value < 0) {
        str[0] = '-';
        return utoa(-value, str + 1, base);
    }
    return utoa(value, str, base);
}

char* itoa_lc(int value, char* str, int base) {
    if(value < 0) {
        str[0] = '-';
        return utoa_lc(-value, str + 1, base);
    }
    return utoa_lc(value, str, base);
}

void* memcpy(void* dest, void* src, size_t num) {
    uint32_t* dest_u32 = dest;
    uint32_t* src_u32 = src;
    size_t num_u32 = num / 4;
    uint8_t* dest_u8 = dest + num_u32 * 4;
    uint8_t* src_u8 = src + num_u32 * 4;
    size_t num_u8 = num % 4;
    for(size_t i = 0; i < num_u32; ++i) {
        dest_u32[i] = src_u32[i];
    }
    for(size_t i = 0; i < num_u8; ++i) {
        dest_u8[i] = src_u8[i];
    }
    return dest;
}

void* memset(void* ptr, int value, size_t num) {
    uint8_t value_u8 = (uint8_t) value;
    uint32_t value_u32 = value_u8 |
            value_u8 << 8 |
            value_u8 << 16 |
            value_u8 << 24;
    size_t num_u32 = num / 4;
    size_t num_u8 = num % 4;
    uint32_t* ptr_u32 = ptr;
    uint8_t* ptr_u8 = ptr + num_u32 * 4;
    for(size_t i = 0; i < num_u32; ++i) {
        ptr_u32[i] = value_u32;
    }
    for(size_t i = 0; i < num_u8; ++i) {
        ptr_u8[i] = value_u8;
    }
    return ptr;
}

void exit(int status) {
    if(status != 0) {
        VgaColorScheme err_color_scheme = {
                .foreground = VGA_COLOR_LIGHT(VGA_COLOR_RED),
                .background = VGA_COLOR_BLACK
        };
        VgaTerminalSwitchColorScheme(err_color_scheme);
        printf("A fatal error has occured, exiting with error code %d\n",
                status);
    }
    __asm volatile("cli");
    while(true) {
        __asm volatile("hlt");
    }
}

int atoi(const char* str) {
    static const char* digits = "0123456789";
    size_t i;
    for(i = 0; isspace(str[i]); i++);
    int sign = 1;
    if(str[i] == '-') {
	    sign = -1;
	    i++;
    } else if(str[i] == '+') {
	    i++;
    }
    int ret = 0;
    for(const char* ptr; (ptr = strchr(digits, str[i])); i++) {
	    if(ptr == NULL) {
    	    break;
	    }
	    ret *= 10;
	    ret += ptr - digits;
    }
    return ret * sign;
}
