/******************************************************************************
 * File:       /vv4os/stdlib/stdlib.h                                         *
 * Author:     velikiyv4                                                      *
 * Created on: Aug 21, 2016                                                   *
 * Purpose:    Provides standard functions.                                   *
 ******************************************************************************/

#pragma once

#include <stddef.h>

char* utoa(unsigned int value, char* str, int base);
char* utoa_lc(unsigned int value, char* str, int base);
char* itoa(int value, char* str, int base);
char* itoa_lc(int value, char* str, int base);
void* memcpy(void* dest, void* src, size_t num);
void* memset(void* ptr, int value, size_t num);
void exit(int status);
