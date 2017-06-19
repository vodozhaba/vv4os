/******************************************************************************
 * File:       /vv4os/stdlib/string.h                                         *
 * Author:     vodozhaba                                                      *
 * Created on: Apr 22, 2017                                                   *
 * Purpose:    Provides str* functions.                                       *
 ******************************************************************************/

#pragma once

#include <stddef.h>

int strcmp(const char* str1, const char* str2);
size_t strlen(const char* str);
char* strcpy(char* dest, const char* src);
