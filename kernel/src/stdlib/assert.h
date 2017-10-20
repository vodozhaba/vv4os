// File:       stdlib/assert.h                                         
// Author:     vodozhaba                                                      
// Created on: Oct 14, 2017                                                   
// Purpose:    Provides the useful assert(bool) macro.                        

#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"

#ifdef NDEBUG
#define assert(b) ((void) 0)
#else
#define assert(b) \
do { \
    if(!(b)) { \
        printf("assert() failed at %s:%d.", __FILE__, __LINE__); \
        while(true); \
    } \
} while(false)
#endif
