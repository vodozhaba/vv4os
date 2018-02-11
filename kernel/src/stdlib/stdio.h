// File:       stdlib/stdio.h
// Author:     vodozhaba
// Created on: Aug 21, 2016
// Purpose:    Provides standard I/O functions.

#pragma once

#include <stdarg.h>
#include "io/disk/file.h"

extern FileDescriptor* stdout;
extern FileDescriptor* stderr;

size_t StdoutWriteOp(FileDescriptor* file, size_t size, const void* buf);
size_t StderrWriteOp(FileDescriptor* file, size_t size, const void* buf);

int putchar(int character);
int isspace (int c);
int fprintf(FileDescriptor* file,  const char* fmt, ...);
int vfprintf(FileDescriptor* file,  const char* fmt, va_list argss);
int printf(const char* fmt, ...);
int sprintf(char* dest, const char* fmt, ...);
