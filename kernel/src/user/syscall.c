// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// File:       user/syscall.c
// Author:     vodozhaba
// Created on: Feb 11, 2018
// Purpose:    The kernel's interface for user processes.

#include "mem/kernel_mem.h"
#include "user/process.h"
#include "user/syscall.h"
#include <stdio.h>

#define FN_CODE_READ 0
#define FN_CODE_WRITE 1

size_t SyscallRead(va_list args) {
    uint32_t local_id = va_arg(args, uint32_t);
    FileDescriptor* file = UserProcessLocalFile(UserProcessCurrent(), local_id);
    if(file == NULL) {
        return -1;
    }
    void* buf = va_arg(args, void*);
    size_t len = va_arg(args, size_t);
    if(buf + len >= KERNEL_STATIC_MEM_START) {
        // What a nasty process!
        return -1;
    }
    return file->read_op(file, len, buf);
}

size_t SyscallWrite(va_list args) {
    uint32_t local_id = va_arg(args, uint32_t);
    FileDescriptor* file = UserProcessLocalFile(UserProcessCurrent(), local_id);
    if(file == NULL) {
        return -1;
    }
    void* buf = va_arg(args, void*);
    size_t len = va_arg(args, size_t);
    if(buf + len >= KERNEL_STATIC_MEM_START) {
        // What a nasty process!
        return -1;
    }
    return file->write_op(file, len, buf);
}

size_t Syscall(size_t fn_code, ...) {
    va_list args;
    va_start(args, fn_code);
    size_t ret;
    switch(fn_code) {
        case FN_CODE_READ:
            ret = SyscallRead(args);
            break;
        case FN_CODE_WRITE:
            ret = SyscallWrite(args);
            break;
        default:
            ret = -1;
    }
    va_end(args);
    return ret;
}