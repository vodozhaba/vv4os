// File:       user/syscall.h
// Author:     vodozhaba
// Created on: Feb 11, 2018
// Purpose:    The kernel's interface for user processes.

#include <stdarg.h>
#include <stddef.h>

size_t Syscall(void* user_state, size_t fn_code, ...);