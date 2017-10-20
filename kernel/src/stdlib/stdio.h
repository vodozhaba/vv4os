// File:       stdlib/stdio.h
// Author:     vodozhaba
// Created on: Aug 21, 2016
// Purpose:    Provides standard I/O functions.

#pragma once

#include <stdarg.h>

#define puts(s) \
    _Pragma("message \"puts() implementation does NOT append '\\\\n'\""); \
    _puts(s);

int putchar(int character);
int _puts(const char* s);
int isspace (int c);
int printf(const char* fmt, ...);
int sprintf(char* dest, const char* fmt, ...);
