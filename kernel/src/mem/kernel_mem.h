// File:       arch/x86/mem/kernel_mem.h
// Author:     vodozhaba
// Created on: Jan 14, 2018
// Purpose:    Provides macros for getting kernel post-linkage info.

#pragma once

extern char _KERNEL_STATIC_MEM_START[];
extern char _KERNEL_STATIC_MEM_END[];

#define KERNEL_STATIC_MEM_START ((void*) _KERNEL_STATIC_MEM_START)
#define KERNEL_STATIC_MEM_END ((void*) _KERNEL_STATIC_MEM_END)
