// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       stdlib/ctype.c
// Author:     vodozhaba
// Created on: Jan 13, 2018
// Purpose:    Provides character-level utility functions.

#include <ctype.h>

int tolower(int c) {
    return (c >= 'A' && c <= 'Z') ? (c + ('a' - 'A')) : c;
}