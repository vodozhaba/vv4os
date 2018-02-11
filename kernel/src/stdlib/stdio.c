// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       stdlib/stdio.c
// Author:     vodozhaba
// Created on: Aug 21, 2016
// Purpose:    Provides standard I/O functions.

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io/disk/file.h"
#include "io/vga_terminal.h"

FileDescriptor _stdin = { .traverse_op = FileTraverseOpStub, .read_op = FileAccessOpStub, .write_op = FileAccessOpStub };
FileDescriptor* stdin = &_stdin;

FileDescriptor _stdout = { .traverse_op = FileTraverseOpStub, .read_op = FileAccessOpStub, .write_op = StdoutWriteOp };
FileDescriptor* stdout = &_stdout;

FileDescriptor _stderr = { .traverse_op = FileTraverseOpStub, .read_op = FileAccessOpStub, .write_op = StderrWriteOp };
FileDescriptor* stderr = &_stderr;

size_t StdoutWriteOp(__attribute__((unused)) FileDescriptor* file, size_t size, const void* buf) {
    VgaTerminalSwitchColorScheme(default_color_scheme);
    const char* str = buf;
    for(size_t i = 0; i < size; i++) {
        putchar(str[i]);
    }
    return size;
}

size_t StderrWriteOp(__attribute__((unused)) FileDescriptor* file, size_t size, const void* buf) {
    VgaTerminalSwitchColorScheme(err_color_scheme);
    const char* str = buf;
    for(size_t i = 0; i < size; i++) {
        putchar(str[i]);
    }
    return size;
}

int putchar(int character) {
    VgaTerminalPut((char) character);
    return character;
}

int isspace (int c) {
    return strchr(" \t\n\v\f\r", c) != NULL;
}

int vfprintf(FileDescriptor* file, const char* fmt, va_list args) {
    for(size_t i = 0; fmt[i]; i++) {
        if(fmt[i] != '%') {
            file->write_op(file, 1, &fmt[i]);
            continue;
        }
        size_t width = 0;
        char c, pad_c = ' ';
    next:
        c = fmt[++i];
        switch(c) {
        case '0': {
    	    pad_c = '0';
    	    goto next; // sorry!
        }
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': {
    	    width = atoi(&fmt[i]);
    	    goto next;
        }
        case '%': {
            file->write_op(file, 1, "%");
            break;
        }
        case 'd':
        case 'i': {
            int arg = va_arg(args, int);
            char str[32];
            itoa(arg, str, 10);
            size_t len = strlen(str);
            if(len < width) {
                for(size_t k = 0; k < width - len; k++) {
                    file->write_op(file, 1, &pad_c);
                }
            }
            file->write_op(file, len, str);
            break;
        }
        case 'u': {
            unsigned int arg = va_arg(args, unsigned int);
            char str[32];
            utoa(arg, str, 10);
            size_t len = strlen(str);
            if(len < width) {
                for(size_t k = 0; k < width - len; k++) {
                    file->write_op(file, 1, &pad_c);
                }
            }
            file->write_op(file, len, str);
            break;
        }
        case 'o': {
            unsigned int arg = va_arg(args, unsigned int);
            char str[32];
            utoa(arg, str, 8);
            size_t len = strlen(str);
            if(len < width) {
                for(size_t k = 0; k < width - len; k++) {
                    file->write_op(file, 1, &pad_c);
                }
            }
            file->write_op(file, len, str);
            break;
        }
        case 'x': {
            unsigned int arg = va_arg(args, unsigned int);
            char str[32];
            utoa_lc(arg, str, 16);
            size_t len = strlen(str);
            if(len < width) {
                for(size_t k = 0; k < width - len; k++) {
                    file->write_op(file, 1, &pad_c);
                }
            }
            file->write_op(file, len, str);
            break;
        }
        case 'X': {
            unsigned int arg = va_arg(args, unsigned int);
            char str[32];
            utoa(arg, str, 16);
            size_t len = strlen(str);
            if(len < width) {
                for(size_t k = 0; k < width - len; k++) {
                    file->write_op(file, 1, &pad_c);
                }
            }
            file->write_op(file, len, str);
            break;
        }
        case 'c': {
            char arg = va_arg(args, int);
                    file->write_op(file, 1, &arg);
            break;
        }
        case 's': {
            char* str = va_arg(args, char*);
            size_t len = strlen(str);
            if(len < width) {
                for(size_t k = 0; k < width - len; k++) {
                    file->write_op(file, 1, &pad_c);
                }
            }
            file->write_op(file, len, str);
            break;
        }
    }
    }
    /* TODO add valid return value */
    return 0;
}

int fprintf(FileDescriptor* file,  const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int ret = vfprintf(file, fmt, args);
    va_end(args);
    return ret;
}

int printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int ret = vfprintf(stdout, fmt, args);
    va_end(args);
    return ret;
}

int sprintf(char* dest, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    size_t j = 0;
    for(size_t i = 0; fmt[i]; i++) {
        if(fmt[i] != '%') {
            dest[j++] = fmt[i];
            continue;
        }
        size_t width = 0;
        char c, pad_c = ' ';
    next:
        c = fmt[++i];
        switch(c) {
            case '0':
        	    pad_c = '0';
        	    goto next; // sorry!
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
        	    width = atoi(&fmt[i]);
        	    goto next;
            case '%': {
                dest[j++] = '%';
                break;
            }
            case 'd':
            case 'i': {
                int arg = va_arg(args, int);
                char str[32];
                itoa(arg, str, 10);
                size_t len = strlen(str);
                if(len < width) {
                    for(size_t k = 0; k < width - len; k++) {
                	    dest[j++] += pad_c;
                    }
                }
                strcpy(&dest[j], str);
                j += strlen(str);
                break;
            }
            case 'u': {
                unsigned int arg = va_arg(args, unsigned int);
                char str[32];
                utoa(arg, str, 10);
                size_t len = strlen(str);
                if(len < width) {
                    for(size_t k = 0; k < width - len; k++) {
                	    dest[j++] += pad_c;
                    }
                }
                strcpy(&dest[j], str);
                j += strlen(str);
                break;
            }
            case 'o': {
                unsigned int arg = va_arg(args, unsigned int);
                char str[32];
                utoa(arg, str, 8);
                size_t len = strlen(str);
                if(len < width) {
                    for(size_t k = 0; k < width - len; k++) {
                	    dest[j++] += pad_c;
                    }
                }
                strcpy(&dest[j], str);
                j += strlen(str);
                break;
            }
            case 'x': {
                unsigned int arg = va_arg(args, unsigned int);
                char str[32];
                utoa_lc(arg, str, 16);
                size_t len = strlen(str);
                if(len < width) {
                    for(size_t k = 0; k < width - len; k++) {
                	    dest[j++] += pad_c;
                    }
                }
                strcpy(&dest[j], str);
                j += strlen(str);
                break;
            }
            case 'X': {
                unsigned int arg = va_arg(args, unsigned int);
                char str[32];
                utoa(arg, str, 16);
                size_t len = strlen(str);
                if(len < width) {
                    for(size_t k = 0; k < width - len; k++) {
                	    dest[j++] += pad_c;
                    }
                }
                strcpy(&dest[j], str);
                j += strlen(str);
                break;
            }
            case 'c': {
                char arg = va_arg(args, int);
                dest[j++] = arg;
                break;
            }
            case 's': {
                char* str = va_arg(args, char*);
                size_t len = strlen(str);
                if(len < width) {
                    for(size_t k = 0; k < width - len; k++) {
                	    dest[j++] += pad_c;
                    }
                }
                strcpy(&dest[j], str);
                j += strlen(str);
                break;
            }
        }
    }
    dest[j] = 0;
    va_end(args);
    return strlen(dest);
}
