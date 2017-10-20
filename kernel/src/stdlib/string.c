// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       stdlib/string.c
// Author:     vodozhaba
// Created on: Apr 22, 2017
// Purpose:    Provides str* functions.

#include "string.h"
#include <stddef.h>

int strcmp(const char* str1, const char* str2) {
    size_t i;
    for(i = 0; str1[i] !='\0' && str2[i] != '\0' && str1[i] == str2[i]; i++);
    if(str1[i] < str2[i])
	    return -1;
    else if(str1[i] == str2[i])
	    return 0;
    else
	    return 1;
}

size_t strlen(const char* str) {
    size_t i;
    for(i = 0; str[i] != 0; i++);
    return i;
}

char* strcpy(char* dest, const char* src) {
    for(size_t i = 0; i < strlen(src) + 1; i++) {
	    dest[i] = src[i];
    }
    return dest;
}

const char* strchr(const char* str, int character) {
    for(size_t i = 0; i < strlen(str); i++) {
	    if(str[i] == character) {
    	    return &str[i];
	    }
    }
    return NULL;
}
