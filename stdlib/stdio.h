/******************************************************************************
 * File:       /vv4os/stdlib/stdio.h                                          *
 * Author:     velikiyv4                                                      *
 * Created on: Aug 21, 2016                                                   *
 * Purpose:    Provides standard I/O functions.                               *
 ******************************************************************************/

#pragma once

#include <stdarg.h>

#define puts(s) \
    _Pragma("message \"puts() implementation does NOT append '\\\\n'\""); \
    _puts(s);

int putchar(int character);
int _puts(const char* s);
int printf(const char* fmt, ...);
