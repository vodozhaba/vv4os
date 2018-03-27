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
#define FN_CODE_EXIT 2
#define FN_CODE_GETPID 3
#define FN_CODE_ISATTY 4
#define FN_CODE_LSEEK 5
#define FN_CODE_FORK 6

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

size_t SyscallExit() {
    RemoveProcess(UserProcessCurrent());
    while(1);
    return 0; // not executed
}

size_t SyscallGetpid() {
    return UserProcessCurrent();
}

size_t SyscallIsatty(va_list args) {
    uint32_t local_id = va_arg(args, uint32_t);
    FileDescriptor* file = UserProcessLocalFile(UserProcessCurrent(), local_id);
    return file->type == FD_TYPE_TTY;
}

size_t SyscallLseek(va_list args) {
    uint32_t local_id = va_arg(args, uint32_t);
    uint32_t offset = va_arg(args, uint32_t);
    uint32_t origin_code = va_arg(args, uint32_t);
    FileDescriptor* file = UserProcessLocalFile(UserProcessCurrent(), local_id);
    uint32_t origin;
    switch(origin_code) {
        case 0:
            origin = 0;
            break;
        case 1:
            origin = file->seek;
            break;
        case 2:
            origin = file->size;
            break;
        default:
            return -1;
    }
    file->seek = origin + offset;
    return file->seek;
}

size_t SyscallFork(void* user_state) {
    Process* parent = GetProcess(UserProcessCurrent());
    void* state = GenReturnProcessState(user_state, 0);
    uint32_t child = CopyProcess(parent, state);
    return child;
}

size_t Syscall(void* user_state, size_t fn_code, ...) {
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
        case FN_CODE_EXIT:
            ret = SyscallExit(args);
            break;
        case FN_CODE_GETPID:
            ret = SyscallGetpid(args);
            break;
        case FN_CODE_ISATTY:
            ret = SyscallIsatty(args);
            break;
        case FN_CODE_LSEEK:
            ret = SyscallLseek(args);
            break;
        case FN_CODE_FORK:
            ret = SyscallFork(user_state);
            break;
        default:
            ret = -1;
    }
    va_end(args);
    return ret;
}